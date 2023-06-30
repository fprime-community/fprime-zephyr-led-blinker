// ======================================================================
// \title  ZephyrRateDriver.cpp
// \author ethanchee
// \brief  cpp file for ZephyrRateDriver component implementation class
// ======================================================================


#include <Zephyr/Drv/ZephyrRateDriver/ZephyrRateDriver.hpp>
#include <FpConfig.hpp>
#include <Fw/Logger/Logger.hpp>

namespace Zephyr {

    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    ZephyrRateDriver ::
        ZephyrRateDriver(
            const char *const compName
        ) : ZephyrRateDriverComponentBase(compName)
    {

    }

    ZephyrRateDriver ::
        ~ZephyrRateDriver()
    {

    }

    void ZephyrRateDriver::configure(U32 intervalMs)
    {
        this->m_interval = intervalMs;
    }

    void ZephyrRateDriver::s_timer(struct k_timer *timer_id)
    {
        Svc::TimerVal now;
        now.take();
        
        // Check if it is time to run the group
        if(this->isConnected_CycleOut_OutputPort(0))
        {
            this->CycleOut_out(0, now);
        }
        this->m_last = now;
    }

    void ZephyrRateDriver::start()
    {
        U32 microseconds = this->m_interval * 1000;
        Fw::Logger::logMsg("Starting base rate group clock with period of %" PRIu32 " microseconds", microseconds);
        k_timer_init(&s_itimer, s_timer, NULL);

        /* start periodic timer */
        k_timer_start(&s_itimer, K_USEC(microseconds), K_NO_WAIT);
    }

    void ZephyrRateDriver::stop()
    {
        k_timer_stop(&s_itimer);
    }

} // end namespace Zephyr
