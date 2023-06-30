#include <Os/Task.hpp>
#include <Fw/Types/Assert.hpp>

#include <zephyr/kernel.h>

namespace Os {

    namespace {

        //! Zephyr thread entry function expects three arguments. This is a glue that creates such function.
        void zephyrEntryWrapper(
              void *p1, //!< Pointer to `Task::TaskRoutineWrapper`
              void *p2, //!< Unused
              void *p3 //!< Unused
          ) {
            Task::TaskRoutineWrapper *task = reinterpret_cast<Task::TaskRoutineWrapper *>(p1);
            // Call the task's entry point
            task->routine(task->arg);
          }
    }

    Task::Task() : m_handle(reinterpret_cast<POINTER_CAST>(nullptr)), m_identifier(0), m_affinity(-1), m_started(false), m_suspendedOnPurpose(false), m_routineWrapper() {}

    Task::TaskStatus Task::start(const Fw::StringBase &name, taskRoutine routine, void* arg, NATIVE_UINT_TYPE priority, NATIVE_UINT_TYPE stackSize,  NATIVE_UINT_TYPE cpuAffinity, NATIVE_UINT_TYPE identifier, void* stack) {
        FW_ASSERT(stack); // Only support statically allocated stacks.
        FW_ASSERT(cpuAffinity == Task::TASK_DEFAULT); // Zephyr SMP support is not implemented.
        FW_ASSERT(routine);

        this->m_name = "T_";
        this->m_name += name;
        this->m_identifier = identifier;
        this->m_started = false;
        this->m_affinity = cpuAffinity;
        this->m_routineWrapper.routine = routine;
        this->m_routineWrapper.arg = arg;

        // If a registry has been registered, register task
        if (Task::s_taskRegistry) {
            Task::s_taskRegistry->addTask(this);
        }
        
        struct k_thread *thread = reinterpret_cast<struct k_thread *>(k_malloc(sizeof(struct k_thread)));
        if (thread == nullptr) {
            return TASK_ERROR_RESOURCES;
        }


        k_tid_t tid = k_thread_create(thread, reinterpret_cast<k_thread_stack_t *>(stack), stackSize, zephyrEntryWrapper, &this->m_routineWrapper, nullptr, nullptr, priority, 0, K_NO_WAIT);
#ifdef CONFIG_THREAD_NAME
        int ret = k_thread_name_set(thread, this->m_name.toChar());
        FW_ASSERT(ret == 0, ret);
#endif
        this->m_handle = reinterpret_cast<POINTER_CAST>(tid);

        // Uncomment to allow threads allocating memory from kernel heap.
        // k_thread_system_pool_assign(thread);
        return TASK_OK;
    }


    Task::~Task() {
        FW_ASSERT(0);
    }

    // void Task::suspend(bool onPurpose) {
    //     FW_ASSERT(0);
    // }

    // void Task::resume() {
    //     FW_ASSERT(0);
    // }

    // bool Task::isSuspended() {
    //     FW_ASSERT(0);
    //     return false;
    // }

    // TaskId Task::getOsIdentifier() {
    //     TaskId T;
    //     return T;
    // }

    // Task::TaskStatus Task::join(void **value_ptr) {
    //     FW_ASSERT(0);
    //     return TASK_UNKNOWN_ERROR;
    // }
}