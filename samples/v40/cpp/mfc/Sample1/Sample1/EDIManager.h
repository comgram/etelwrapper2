/*
 * EDIManager.h
 *
 * Copyright (c) 2012-2021 ETEL SA. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of ETEL SA 
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
 * REMAINS WITH THE LICENSEE. IF ETEL SHOULD NEVERTHELESS BE FOUND LIABLE, WHETER DIRECTLY OR INDRECTLY, FOR ANY LOSS, 
 * DAMAGE OR INJURY ARISING UNDER THIS AGREEMENT OR OTHERWISE, REGARDLESS OF CAUSE OR ORIGIN, ON ANY BASIS WHATSOEVER, 
 * ITS TOTAL MAXIMUM LIABILITY IS LIMITED TO CHF 100.000 WHICH WILL BE THE COMPLETE AND EXCLUSIVE REMEDY AGAINST ETEL.

 * This software is in particular not designed or intended .for use in on-line control of aircraft, air traffic, aircraft 
 * navigation or aircraft communications; or in the design, construction, Operation or maintenance of any nuclear facility. 
 * Licensee represents and warrants that it will not use or redistribute the Software for such purposes.
 *
 */


// This class: "EDIManager" will be the one in charge of managing the EDI api layer. 
// We have applied a Singleton dessing pattern strategy in order to give all the manager
// responsabilities of the EDI layer to a single instance of this class. Thus, different
// and independent presentation resources (dialogs) could access to the edi api through the
// same single interface instance.

#pragma once

/*** standard libraries ***/

#include <stdio.h>  /* the standard I/O library              */
#include <string>	/* standard string library				 */

// A try-catch exception policy will be achieved in the EDI API calls. Since the EDI layer returns
// an error, this will be catched and propagated to the Presentation MFC Dialog layer.
#include <exception>/* standard c++ exceptions library		 */

#include <stdlib.h>	/* standard general purpose library      */
#include <ctype.h>  /* standard character conversion library */
#include <math.h>   /* standard math interface; used for random numbers here*/
#include <conio.h>  /* standard library for getch()*/

/*** ETEL libraries ***/
#define ETEL_OO_API
#include <dsa40.h>  /* EDI high level library version 4.0x.  */
#include <dmd40.h>  /* EDI constants definitions             */


using namespace std;

class EDIManager
{
public:

	/* Data Structure sent to the Presentation Layer to achieve the monitoring thread controlling values: position,
	 * velocity and status of the motor. */
	struct MonitoringData
	{
		double pos;
		double vel;
		DsaStatus status;

		MonitoringData():
		pos(0.0), vel(0.0)
		{}
	};

	// Function to get the single instance of this class. Since it is not an instance dependent call but a class dependent call it has to be
	// a static function. 
	static EDIManager* getEDIMngrInstance();

	/* Several functions to achieve a basic interaction with the Drive System as it could be: initialize the drive, establish a connection,
	 * disconnect the drive, power it on and off, and send basical movement orders to the motor.
	 */

	void InitDriveSystem();
	void ConnectDriveSystem(string sConnectionMode);
	void DisconnectDriveSystem();
	void PwrOnDriveSystem();
	void PwrOffDriveSystem();
	void Homing();
	void MoveMinPos();
	void MoveMaxPos();
	void MoveStepPlus();
	void MoveStepMinus();
	void QuickStop();

	/* Thread to monitorize values as position, velocity and status of the motor. */
	void DisplayThread(MonitoringData& monitorData);

protected:
	
	EDIManager(void);
	~EDIManager(void);

private:

	// Singleton instance of EDIManager class to access to the EDI API. It is a class member variable instead of an instance member variable
	// thus it has to be defined as static.
	static EDIManager* m_pInstance;

	 /*
     * Most functions of the library return an error code or 0 if no error
     * occured. We need a variable to store the last error.
     * The error codes are negative values, ranging from -399 to -300 for the
     * dsa40 library.
     */
    int err;				

    /*
     * We need two variables to hold the minimum and maximum position of the
     * motor. We are going to make movements within these bounds.
     * All parameters which represent physical dimensions are usually
     * defined in ISO units and use double precision floating point variables.
     */
    double m_vdPosMin, m_vdPosMax;
    /*
     * Moving distances will also be expressed in relation to the total range of
     * motion defined by the above limits.
     */
    double m_vdRangeOfMotion;

  /*** Creating the objects ***/

    /*
     * We must also define a pointer to a DSA_DRIVE object for each drive (in this case
	 * we will only have the drive controlling the X Axis). This "hidden"
     * object is defined in the dsa40 library. The client does not need to access
     * the members of this object directly, but need to pass it to various dsa
     * library functions. It can be compared to the FILE structure defined in
     * the standard I/O library.
     * The pointers *must* be initialized to NULL before calling the created
     * drive function, otherwise this function will fail.
     */
    DsaDrive *pAxisX;

	// Member variable to control the access to the callback calls. Since we are making Polling after asynchronous calls
	// we need a variable to control if the callback call has been executed or not.
	static ebool m_bCallbackCalled;

	// Class member variable to check if the asyncronous call has produced an error.
	static int m_iAsyncFuncError;

	// Function to destroy the singleton instance properly.
	static void DestroyEDIMngr();

	// Callback function for the Asynchronous EDI functions.
	static void Callback(DSA_DEVICE_BASE *dev, int err, void *param);
	// Function to handle the different errors launched by the EDI API.
	void errorHandle(int a_iErr, DsaDrive *a_pDrive);

	void errorHandle();

	// Function to synchronize a point of the code with the end of the asynchronous calls to continue the execution of the
	// program.
	void Polling();

	// Function to check if a drive is a valid one and if it is open.
	ebool isActivatedDrive(DsaDrive a_Axis);

};
