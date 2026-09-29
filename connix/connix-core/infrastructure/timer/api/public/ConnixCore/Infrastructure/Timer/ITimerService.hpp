#pragma once

#include <memory>
#include <string>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Central service interface for managing registered timers and event
 * pumping.
 */
class ITimerService
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ITimerService() = default;

    /**
     * @brief Registers a timer instance, assigning and returning a unique
     * timer identifier.
     * @param timer Shared pointer to the timer instance to register.
     * @return Generated unique timer identifier.
     */
    virtual std::string registerTimer(std::shared_ptr<ITimer> timer) = 0;

    /**
     * @brief Stops and unregisters the named timer from the service
     * collection.
     * @param timerId Identifier of the timer to remove.
     * @throws TimerException if timerId is not found.
     */
    virtual void unregisterTimer(const std::string& timerId) = 0;

    /**
     * @brief Retrieves a registered timer instance by identifier.
     * @param timerId Identifier of the timer to retrieve.
     * @return Shared pointer to the registered ITimer instance.
     * @throws TimerException if timerId is not found.
     */
    virtual std::shared_ptr<ITimer>
    getTimer(const std::string& timerId) const = 0;

    /**
     * @brief Checks if a timer with the given identifier is currently
     * registered.
     * @param timerId Identifier to query.
     * @return True if registered, false otherwise.
     */
    virtual bool hasTimer(const std::string& timerId) const = 0;

    /**
     * @brief Halts all registered timers simultaneously.
     */
    virtual void stopAll() = 0;

    /**
     * @brief Registers a centralized subscriber callback to receive timer
     * expiration events.
     * @param handler Function to invoke when any registered timer expires.
     */
    virtual void setEventHandler(TimerEventHandler handler) = 0;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
