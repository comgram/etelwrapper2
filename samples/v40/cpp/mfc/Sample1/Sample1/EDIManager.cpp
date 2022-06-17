/*
 * EDIManager.cpp
 *
 * Copyright (c) 1997-2020 ETEL SA. All Rights Reserved.
 *
 * This software is the confidential and proprietary informatione of ETEL SA
 * ("Confidential Information"). You shall not disclose such Confidential
 * Information and shall use it only in accordance with the terms of the
 * license agreement you entered into with ETEL.
 *
 * This software is provided "AS IS" without a warranty or representations of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED WARRANTY
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT ARE HEREBY EXCLUDED. ETEL AND ITS
 * LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL ETEL OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR
 * DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 * OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF ETEL HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES. THE ENTIRE RISK ARISING OUT OF USE, PERFORMANCE OR NON-PERFORMANCE OF THE SOFTWARE
 * REMAINS WITH THE CUSTOMER. IF ETEL SHOULD NEVERTHELESS BE FOUND LIABLE, WHETER DIRECTLY OR INDRECTLY, FOR ANY LOSS,
 * DAMAGE OR INJURY ARISING UNDER THIS AGREEMENT OR OTHERWISE, REGARDLESS OF CAUSE OR ORIGIN, ON ANY BASIS WHATSOEVER,
 * ITS TOTAL MAXIMUM LIABILITY IS LIMITED TO CHF 100.000 WHICH WILL BE THE COMPLETE AND EXCLUSIVE REMEDY AGAINST ETEL.

 * This software is in particular not designed or intended .for use in on-line control of aircraft, air traffic, aircraft
 * navigation or aircraft communications; or in the design, construction, Operation or maintenance of any nuclear facility.
 * Licensee represents and warrants that it will not use or redistribute the Software for such purposes.
 */

#include "EDIManager.h"
#include "StdAfx.h"

// Initialization of the static member variables
EDIManager *EDIManager::m_pInstance       = NULL;
ebool       EDIManager::m_bCallbackCalled = false;
int         EDIManager::m_iAsyncFuncError = 0;

EDIManager::EDIManager(void) : pAxisX(NULL)
{
}

EDIManager::~EDIManager(void)
{
}

// Function to get the single instance of this class.
// This is a singleton pattern dessing, therefore we will define the destruction strategy of the singleton instance
// at the exit of the use of this class.
EDIManager *EDIManager::getEDIMngrInstance()
{
    if (!m_pInstance) {
        m_pInstance = new EDIManager();
        atexit(DestroyEDIMngr);
    }

    return m_pInstance;
}

// Destruction of the singleton EDIManager.
void EDIManager::DestroyEDIMngr()
{
    if (m_pInstance != NULL)
        delete m_pInstance;

    m_pInstance = NULL;
}

// Function to synchronize a point of the code with the end of the asynchronous calls to continue the execution of the
// program. We will check if there has been an error in the asynchronous function call here.
void EDIManager::Polling()
{
    ///* Wait till callback function is called */
    while (!m_bCallbackCalled)
        Sleep(100);

    if (m_iAsyncFuncError)
        errorHandle(m_iAsyncFuncError, pAxisX);
}

// Function to check if a drive is a valid one and if it is open.
ebool EDIManager::isActivatedDrive(DsaDrive a_Axis)
{
    return a_Axis.isOpen();
}

// Creation of the Drive
void EDIManager::InitDriveSystem()
{
    if (!pAxisX)
        pAxisX = new DsaDrive();
}

// Connection of the drive. We will check if it is a valid one, and if it is not already open in order to execute the dsa_open_u function.

void EDIManager::ConnectDriveSystem(string sConnectionMode)
{
    if (!pAxisX)
        errorHandle();
    if (!isActivatedDrive(*pAxisX)) {
        /* Now we are ready to open the communication channels.
         * See � on Establishing the communication in the Users's Manual for
         * more details on the syntax involved with opening the communication.
         */
        string sConnection = string("etb:") + sConnectionMode + string(":0");

        pAxisX->open(sConnection.c_str());
    }
}

// Disconnection of the Drive System
void EDIManager::DisconnectDriveSystem()
{
    if (!pAxisX)
        return;
    if (isActivatedDrive(*pAxisX))
        pAxisX->close();
    delete pAxisX;
    pAxisX = NULL;
}

/*** Powering up the Drive - Motor Axis X ***/

void EDIManager::PwrOnDriveSystem()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    /* Different devices take different times to completely startup which means that in
     * a system that includes different types of devices, such as some position
     * controllers and a position motion controller, some devices will be ready when
     * others are not. This can have the effect of putting in error a device that expects
     * another to be there but that hasn�t finished starting up. This is why, when all
     * devices have finished their activation, it is recommended to start the application by
     * a general �reset error� command
     */
    m_bCallbackCalled = false;
    pAxisX->resetErrorEx(0, Callback, (void *)1);
    /* Wait until callback function is called */
    Polling();

    /* Do an asynchronous power on */
    /* Handler will be called once power on function is finished */
    /* Meanwhile, the application can do something, in this case displaying */
    /* to indicate status, position and velocity monitoring values. */
    /* This function will return BEFORE the controllers terminate the PWR=1 function.
     * The Callback function will be executed once the controllers has terminated PWR=1 function.
     * Then we will wait the end of the Callback function with a polling strategy.
     */
    m_bCallbackCalled = false;
    pAxisX->powerOn(Callback, (void *)2);
    /* Wait until callback function is called */
    Polling();
}

// End of movements: power motors off:
void EDIManager::PwrOffDriveSystem()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    pAxisX->powerOff(60000);
}

// Homing reference Process
void EDIManager::Homing()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    /* Next step in readying the controllers and motors for operation is performing the
     * homing procedure in order to find the reference for the motor�s absolute position.
     */
    pAxisX->homingStart(10000);

    /* Now, before we can make other movements, we must wait until homing is terminated. Meanwhile,
     * we can continue executing the monitoring thread, therefore we will launch an asynchronous call.
     */
    m_bCallbackCalled = false;
    pAxisX->waitMovement(Callback, (void *)3);

    /*
     * During the indexation procedure, we can read the minimum and maximum
     * position allowed for the motor. This is done by calling the function
     * "get min/max soft position limit". This function writes the data at the
     * address provided in the second argument. The third argument indicates
     * to the function what kind of value must be returned: the current value,
     * the default value, the maximum or minimum value,... Some of these
     * operations (getting min, max or default values) do not involve any
     * communication.
     * As usual, a timeout must be provided, but for request operation,
     * which are always executed in one cycle in the drive, it is recommended
     * to ask for the default timeout. This special timeout value is
     * automatically adjusted to the communication channel used.
     */
    m_vdPosMin = pAxisX->getMinSoftPositionLimit();
    m_vdPosMax = pAxisX->getMaxSoftPositionLimit();

    // We get the range of the motion
    m_vdRangeOfMotion = m_vdPosMax - m_vdPosMin;

    /* Wait until callback of homing waitMovement function is called */
    Polling();

    /* A position controlled movement can be defined by up to four parameters. The speed,
     * acceleration and jerk time describing the movement profile can be specified.
     * The movement is started when setting the target position
     */

    double profileSpeed = 0.1;  /* m/s (or turns/secs) for rotary motors */
    double profileAcc   = 2.;   /* m/s^2 or rad/s^2 */
    double jerkTime     = 0.01; /* seconds */

    pAxisX->setProfileVelocity(0, profileSpeed);
    pAxisX->setProfileAcceleration(0, profileAcc);
    pAxisX->setJerkTime(0, jerkTime);
}

// Moving to the min position
void EDIManager::MoveMinPos()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    /* Making motors simply go to a given position can be as straight forward as calling
     * dsa_set_target_position_s() which is the equivalent of the controller�s POS.
     */
    pAxisX->setTargetPosition(0, m_vdPosMin);

    /* Now, before we can make other movements, we must wait until it is terminated. To this
     * end, we wait until the movement is finished, in an asynchronous mode to allow the monitoring thread
     * execution.
     */
    pAxisX->waitMovement(Callback, (void *)4);

    // We do not make polling because there is no function to execute after. So, we do not have to
    // get a synchronization code point.
}

// Moving to the max position
void EDIManager::MoveMaxPos()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    /* Making motors simply go to a given position can be as straight forward as calling
     * dsa_set_target_position_s() which is the equivalent of the controller�s POS.
     */
    pAxisX->setTargetPosition(0, m_vdPosMax);

    /* Now, before we can make other movements, we must wait until it is terminated. To this
     * end, we wait until the movement is finished, in an asynchronous mode to allow the monitoring thread
     * execution.
     */
    pAxisX->waitMovement(Callback, (void *)5);

    // We do not make polling because there is no function to execute after. So, we do not have to
    // get a synchronization code point.
}

// Moving a step in the positive direction
void EDIManager::MoveStepPlus()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    double dCurrentPos = pAxisX->getPositionActualValue(Dsa::GET_CURRENT);

    if ((dCurrentPos + (m_vdRangeOfMotion / 10)) <= m_vdPosMax) {
        /* Making motors simply go to a given position can be as straight forward as calling
         * dsa_set_target_position_s() which is the equivalent of the controller�s POS.
         * We will execute a step movement of the 10% of the range of motion.
         */
        pAxisX->setTargetPosition(0, (dCurrentPos + (m_vdRangeOfMotion / 10)));

        /* Now, before we can make other movements, we must wait until it is terminated. To this
         * end, we wait until the movement is finished, in an asynchronous mode to allow the monitoring thread
         * execution.
         */
        pAxisX->waitMovement(Callback, (void *)6);

        // We do not make polling because there is no function to execute after. So, we do not have to
        // get a synchronization code point.
    }
}

// Moving a step in the negative direction
void EDIManager::MoveStepMinus()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    double dCurrentPos = pAxisX->getPositionActualValue(Dsa::GET_CURRENT);

    if ((dCurrentPos - m_vdRangeOfMotion / 10) > m_vdPosMin) {
        /* Making motors simply go to a given position can be as straight forward as calling
         * dsa_set_target_position_s() which is the equivalent of the controller�s POS.
         * We will execute a step movement of the 10% of the range of motion.
         */
        pAxisX->setTargetPosition(0, (dCurrentPos - m_vdRangeOfMotion / 10));

        /* Now, before we can make other movements, we must wait until it is terminated. To this
         * end, we wait until the movement is finished, in an asynchronous mode to allow the monitoring thread
         * execution.
         */
        pAxisX->waitMovement(Callback, (void *)7);

        // We do not make polling because there is no function to execute after. So, we do not have to
        // get a synchronization code point.
    }
}

void EDIManager::QuickStop()
{
    if (!pAxisX || !isActivatedDrive(*pAxisX))
        return;

    /* Stop movements. */
    pAxisX->quickStop(Dsa::QS_PROGRAMMED_DEC, Dsa::QS_BYPASS | Dsa::QS_STOP_SEQUENCE);

    /* Now, before we can make other movements, we must wait until it is terminated. To this
     * end, we wait until the movement is finished, in an asynchronous mode to allow the monitoring thread
     * execution.
     */
    m_bCallbackCalled = false;
    pAxisX->waitMovement(Callback, (void *)8);
    // After QuickStop we will usually execute PwrOffDriveSystem, so we will synchronize at this point with a polling in order to be sure
    // of finishing the movement before powering the motor off.
    Polling();
}

/*** Monitoring data ***/

/*
 * Display thread function.
 */
void EDIManager::DisplayThread(MonitoringData &monitorData)
{
    /*
     * Each 100 ms, we will display the status and the position of each
     * drive. As usually, the status structure must be initialized before
     * using it.
     */

    double    pos    = 0.0;
    double    vel    = 0.0;
    DsaStatus status = {sizeof(DSA_STATUS)};

    /*
     * Get the drive status. Theses functions don't generate any activity
     * on the communication channel. The status of each drive is always
     * kept up-to-date in the process memory, which means that theses
     * function are very effective and could be often called without
     * degrading performances.
     */
    if (pAxisX && isActivatedDrive(*pAxisX)) {
        /* Get the status of axis X. */
        status = pAxisX->getStatus();

        /* Get the position of axis X. */
        pos = pAxisX->getPositionActualValue();

        /* Get the velocity of axis X. */
        vel = pAxisX->getVelocityActualValue();
    }

    // We get the retrieving values in the monitoring data structure
    monitorData.pos    = pos;
    monitorData.vel    = vel;
    monitorData.status = status;

    /*
     * We can now stop for 100ms - the following call is platform dependent.
     */
    Sleep(100);

    return;
}

/*
 * callback function.
 * This function will be called once asynchronous call is terminated
 */
void EDIManager::Callback(DSA_DEVICE_BASE *dev, int err, void *param)
{
    /* The asynchronous function has terminated with an error */
    if (err) {
        m_iAsyncFuncError = err;
        printf("\n-> callback function called with error %d and parameter %lld\n", err, (long long)(param));
    }
    /* The asynchronous function has terminated without error */
    else {
        m_iAsyncFuncError = 0;
        printf("\n-> callback function called with parameter %lld\n", (long long)(param));
    }
    m_bCallbackCalled = true;
}

// Function to handle the different errors launched by the EDI API. It is the function which initiates the exception propagation policy,
// sending the error message until the Presentation layer (Message Dialog Box).
void EDIManager::errorHandle(int a_iErr, DsaDrive *a_pDrive)
{
    char strError[1024];

    a_pDrive->extSdiag(1024, strError, __FILE__, __LINE__, a_iErr);
    throw exception(strError);
}

void EDIManager::errorHandle()
{
    throw exception("Drive is not detected.");
}