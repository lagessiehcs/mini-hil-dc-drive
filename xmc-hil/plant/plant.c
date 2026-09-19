#include <xmc_ccu4.h>
#include <xmc_gpio.h>

#define PLANT_RATE_HZ 1000U
#define TIMER_PRESCALER 2U
#define LED_TOGGLE_TICKS 500U

#define PLANT_STEP_S (1.0f / PLANT_RATE_HZ)
#define PLANT_TICK_BUDGET_CYCLES (SystemCoreClock / PLANT_RATE_HZ)

static volatile float armature_current_a = 0.0f;
static volatile float angular_speed_rad_s = 0.0f;

static volatile float command_voltage_v = 0.0f;
static volatile float load_torque_nm = 0.0f;

static volatile uint32_t plant_tick_count = 0U;
static volatile uint32_t overrun_count = 0U;
static volatile uint32_t plant_last_execution_cycles = 0U;
static volatile uint32_t plant_max_execution_cycles = 0U;
static volatile uint64_t plant_total_execution_cycles = 0U;
static volatile uint32_t plant_execution_samples = 0U;

static const XMC_CCU4_SLICE_COMPARE_CONFIG_t timer_config = {
    .timer_mode = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
    .monoshot = XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT,
    .shadow_xfer_clear = 0U,
    .dither_timer_period = 0U,
    .dither_duty_cycle = 0U,
    .prescaler_mode = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
    .mcm_enable = 0U,
    .prescaler_initval = XMC_CCU4_SLICE_PRESCALER_2,
    .float_limit = 0U,
    .dither_limit = 0U,
    .passive_level = XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .timer_concatenation = 0U};

static void plant_step(void)
{
  const float voltage_v = command_voltage_v;
  const float load_nm = load_torque_nm;

  const float current_a = armature_current_a;
  const float speed_rad_s = angular_speed_rad_s;

  const float current_derivative =
      (voltage_v - 2.0f * current_a - 0.05f * speed_rad_s) / 0.005f;

  const float speed_derivative =
      (0.05f * current_a - 0.0001f * speed_rad_s - load_nm) / 0.0001f;

  armature_current_a = current_a + PLANT_STEP_S * current_derivative;
  angular_speed_rad_s = speed_rad_s + PLANT_STEP_S * speed_derivative;
}

void CCU40_0_IRQHandler(void)
{
  static uint32_t heartbeat_ticks = 0U;
  const uint32_t start_cycles = DWT->CYCCNT;
  uint32_t execution_cycles;

  XMC_CCU4_SLICE_ClearEvent(
      CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

  plant_tick_count++;
  plant_step();

  execution_cycles = DWT->CYCCNT - start_cycles;
  plant_last_execution_cycles = execution_cycles;
  plant_total_execution_cycles += execution_cycles;
  plant_execution_samples++;

  if (execution_cycles > plant_max_execution_cycles)
  {
    plant_max_execution_cycles = execution_cycles;
  }

  if (execution_cycles > PLANT_TICK_BUDGET_CYCLES)
  {
    overrun_count++;
  }

  heartbeat_ticks++;

  if (heartbeat_ticks >= LED_TOGGLE_TICKS)
  {
    heartbeat_ticks = 0U;
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
  }
}

static void init_led(void)
{
  const XMC_GPIO_CONFIG_t led_config = {
      .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
      .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
      .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

  XMC_GPIO_Init(XMC_GPIO_PORT1, 0, &led_config);
}

static void init_cycle_counter(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0U;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void init_plant_timer(void)
{
  const uint32_t period =
      (SystemCoreClock / (TIMER_PRESCALER * PLANT_RATE_HZ)) - 1U;

  XMC_CCU4_SetModuleClock(CCU40, XMC_CCU4_CLOCK_SCU);
  XMC_CCU4_Init(CCU40, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
  XMC_CCU4_EnableClock(CCU40, 0U);

  XMC_CCU4_SLICE_CompareInit(CCU40_CC40, &timer_config);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC40, period);
  XMC_CCU4_SLICE_EnableEvent(
      CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
  XMC_CCU4_SLICE_SetInterruptNode(
      CCU40_CC40,
      XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH,
      XMC_CCU4_SLICE_SR_ID_0);
  XMC_CCU4_EnableShadowTransfer(CCU40, XMC_CCU4_SHADOW_TRANSFER_SLICE_0);

  NVIC_SetPriority(CCU40_0_IRQn, 10U);
  NVIC_EnableIRQ(CCU40_0_IRQn);

  XMC_CCU4_SLICE_StartTimer(CCU40_CC40);
}

int main(void)
{
  init_led();
  init_cycle_counter();
  init_plant_timer();

  while (1)
  {
  }
}
