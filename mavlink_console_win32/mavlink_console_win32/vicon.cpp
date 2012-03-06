#include "Client.h"
#include <iostream>
#include <windows.h>
#include <math.h>

#include "vicon.h"

using namespace std;
using namespace ViconDataStreamSDK::CPP;

#define	M_PI				3.141592

// Vicon client setup parameters
Client		MyClient;

// fail safe mode
bool		flag_error = true;
double								sT = 0.01;						// Sampling time of controller (Automatically calculated, do not touch, change frequency)
bool		flag = true;
double		x_head[3], x_tail[3], y_head[3], y_tail[3];


double		phiA, thetaA, psiA, xA, yA, zA;
double		phi_c, theta_c, psi_c, x_norm, y_norm;
double		xA_, xA_hat, xA_hat_, xA_dot, xA_dot_;
double		yA_, yA_hat, yA_hat_, yA_dot, yA_dot_;
double		zA_, zA_hat, zA_hat_, zA_dot, zA_dot_;
double		phiA_, phiA_hat, phiA_hat_, phiA_dot, phiA_dot_;
double		thetaA_, thetaA_hat, thetaA_hat_, thetaA_dot, thetaA_dot_;
double		psiA_, psiA_hat, psiA_hat_, psiA_dot, psiA_dot_;

double		phiB, thetaB, psiB, xB, yB, zB;
double		xB_, xB_hat, xB_hat_, xB_dot, xB_dot_;
double		yB_, yB_hat, yB_hat_, yB_dot, yB_dot_;
double		zB_, zB_hat, zB_hat_, zB_dot, zB_dot_;
double		phiB_, phiB_hat, phiB_hat_, phiB_dot, phiB_dot_;
double		thetaB_, thetaB_hat, thetaB_hat_, thetaB_dot, thetaB_dot_;
double		psiB_, psiB_hat, psiB_hat_, psiB_dot, psiB_dot_;

double		wn_a = 50, zeta_a = 0.3, wn_p = 50, zeta_p = 0.3, wn_g = 30, zeta_g = 0.5;
double		k11, k12, k21, k22, k31, k32, k41, k42;


unsigned int pitta_obj_id = 0;
string pitta_name;

void initialize_VICON( void )
{
	int trying = 0;

	cout << " Connecting to Giganet......" << endl;


	// ============================= Connect to Giganet (Vicon motion systems) ============================= 
	while( !MyClient.IsConnected().Connected )
	{
		if(trying++ > 3)
			break;

		Output_Connect _Output_Connect = MyClient.Connect( "localhost:801" );
		Sleep(200);
	}

	if(trying > 3)
		return;

	Output_EnableMarkerData          _Output_EnableMarkerData          = MyClient.EnableMarkerData();
	Output_EnableUnlabeledMarkerData _Output_EnableUnlabeledMarkerData = MyClient.EnableUnlabeledMarkerData();
	Output_EnableDeviceData          _Output_EnableDeviceData          = MyClient.EnableDeviceData();


	Output_SetStreamMode _Output_SetStreamMode = MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );

	cout << " Giganet successfully connected." << endl << endl;

	/// Wait until frame is arrived.
	while( MyClient.GetFrame().Result != Result::Success );

	/// Get number of object shown in vicon
	unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;

	cout << "[NOTICE] " << SubjectCount << " objects found" << endl;

	for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex )
	{
		cout << "Object name : " << MyClient.GetSubjectName(SubjectIndex).SubjectName << endl;

		if(MyClient.GetSubjectName(SubjectIndex).SubjectName == "pitta")
		{
			cout << "Pitta found ID = " << SubjectIndex << endl;
			pitta_obj_id = SubjectIndex;
			pitta_name = MyClient.GetSubjectName(SubjectIndex).SubjectName;
		}
	}
}

void retrieve_VICON( void )
{
	while( MyClient.GetFrame().Result != Result::Success );

	// Count the number of markers
	unsigned int MarkerCount = MyClient.GetMarkerCount(pitta_name).MarkerCount;

	for( unsigned int MarkerIndex = 0 ; MarkerIndex < MarkerCount ; ++MarkerIndex )
	{
		// Get the marker name
		string MarkerName = MyClient.GetMarkerName( pitta_name, MarkerIndex ).MarkerName;

		// Get the global marker translation
		Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
			MyClient.GetMarkerGlobalTranslation(pitta_name, MarkerName );

		if(MarkerName == "1")
		{
			//x_head
			x_head[0] = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
			x_head[1] = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
			x_head[2] = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];
		}

		if(MarkerName == "3")
		{
			//y_head
			y_head[0] = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
			y_head[1] = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
			y_head[2] = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];
		}

		if(MarkerName == "2")
		{
			//x_tail
			x_tail[0] = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
			x_tail[1] = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
			x_tail[2] = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];
		}

		if(MarkerName == "4")
		{
			//y_tail
			y_tail[0] = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
			y_tail[1] = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
			y_tail[2] = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];
		}
	}

	// Position and altitude data
	xA = ((y_tail[0]+y_head[0]+x_tail[0]+x_head[0])/4)/1000; // convert it to meter.
	yA = ((y_tail[1]+y_head[1]+x_tail[1]+x_head[1])/4)/1000;
	zA = ((y_tail[2]+y_head[2]+x_tail[2]+x_head[2])/4)/1000;

	x_norm = sqrt( (x_head[0]-x_tail[0])*(x_head[0]-x_tail[0]) + (x_head[1]-x_tail[1])*(x_head[1]-x_tail[1]) + (x_head[2]-x_tail[2])*(x_head[2]-x_tail[2]) );
	y_norm = sqrt( (y_head[0]-y_tail[0])*(y_head[0]-y_tail[0]) + (y_head[1]-y_tail[1])*(y_head[1]-y_tail[1]) + (y_head[2]-y_tail[2])*(y_head[2]-y_tail[2]) );

	theta_c = asin( (x_head[2]-x_tail[2])/x_norm );
	psi_c = asin( -((x_head[1]-x_tail[1])/x_norm)/cos(theta_c) );
	phi_c = asin( ((y_head[2]-y_tail[2])/y_norm)/cos(theta_c) );

	phiA = phi_c;
	thetaA = theta_c;
	psiA = psi_c;

	RK_integration( &xA_hat , &xA_hat_ , &xA_dot , &xA_dot_ , &xA , &xA_ , wn_p , zeta_p );	// xA_dot = velocity.
	RK_integration( &yA_hat , &yA_hat_ , &yA_dot , &yA_dot_ , &yA , &yA_ , wn_p , zeta_p ); // yA_dot = velocity.
	RK_integration( &zA_hat , &zA_hat_ , &zA_dot , &zA_dot_ , &zA , &zA_ , wn_p , zeta_p ); // zA_dot = velocity. 

	RK_integration( &phiA_hat , &phiA_hat_ , &phiA_dot , &phiA_dot_ , &phiA , &phiA_ , wn_a , zeta_a );
	RK_integration( &thetaA_hat , &thetaA_hat_ , &thetaA_dot , &thetaA_dot_ , &thetaA , &thetaA_ , wn_a , zeta_a );
	RK_integration( &psiA_hat , &psiA_hat_ , &psiA_dot , &psiA_dot_ , &psiA , &psiA_ , wn_a , zeta_a );

	if( phiA_dot < 5000 && phiA_dot > -5000 && thetaA_dot < 5000 && thetaA_dot > -5000 && psiA_dot < 5000 && psiA_dot > -5000)
	{
		flag_error = true;
		//xA_hat = yA_hat = zA_hat = xA_hat_ = yA_hat_ = zA_hat_ = xA_dot_ = yA_dot_ = zA_dot_ = xA_ = yA_ = zA_ = phidA_ = phidA_dot_ = phidA_c_ = thetadA_ = thetadA_dot_ = thetadA_c_ = 0.1;
	}
	else
	{
		flag_error = false;
	}
}

void RK_integration( double* q1 , double* q1_ , double* q2 , double* q2_ , double* sen , double* sen_ , double wn , double zeta )
{
	if( flag == true )
	{
		*q1 = *sen;
		*q2 = 0;
		flag = false;
	}

	k11 = *q2_;
	k12 = -wn*wn*(*q1_) - 2*zeta*wn*(*q2_) + wn*wn*(*sen_);

	k21 = *q2_ + 0.5*k12*sT;
	k22 = -wn*wn*(*q1_+0.5*k11*sT) - 2*zeta*wn*(*q2_+0.5*k12*sT) + wn*wn*(*sen_+0.5*((*sen)-(*sen_)));

	k31 = *q2_ + 0.5*k22*sT;
	k32 = -wn*wn*(*q1_+0.5*k21*sT) - 2*zeta*wn*(*q2_+0.5*k22*sT) + wn*wn*(*sen_+0.5*((*sen)-(*sen_)));

	k41 = *q2_ + k32*sT;
	k42 = -wn*wn*(*q1_+k31*sT) - 2*zeta*wn*(*q2_+k32*sT) + wn*wn*(*sen);

	*q1 = *q1_ + sT/6*( k11 + k21 + k31 + k41 );
	*q2 = *q2_ + sT/6*( k12 + k22 + k32 + k42 );

	*q1_ = *q1;
	*q2_ = *q2;
	*sen_ = *sen;
}

void view_data( void )
{
	//return;
	// View attitude data
	system("cls");
	//printf(" Time      =   %2.3f\t(s)\n",t);
	//printf(" cont_cnt  =   %d\n",control_cnt);

	if( flag_error == true )
	{
		printf(" ===================================================================== \n");
		printf(" ||                !!!!!! WARNING !!!!!!                            || \n");
		printf(" ||Sensor data is NOT valid!!! Please restart this program!!!       || \n");
		printf(" ===================================================================== \n");
	}

	printf("\n");
	
	printf(" x         =   %3.3f\t(cm)\n",xA*100);
	printf(" y         =   %3.3f\t(cm)\n",yA*100);
	printf(" z         =   %3.3f\t(cm)\n",zA*100);		
	printf(" phi       =   %3.3f\t(deg)\n",phiA*180/M_PI);		
	printf(" theta     =   %3.3f\t(deg)\n",thetaA*180/M_PI);		
	printf(" psi       =   %3.3f\t(deg)\n",psiA*180/M_PI);

	printf("Current velocity");
	printf(" x_dot     =   %3.3f\t(cm/s)\n",xA_dot*100);
	printf(" y_dot     =   %3.3f\t(cm/s)\n",yA_dot*100);
	printf(" z_dot     =   %3.3f\t(cm/s)\n",zA_dot*100);
	printf(" phi_dot   =   %3.3f\t(deg/s)\n",phiA_dot*180/M_PI);
	printf(" theta_dot =   %3.3f\t(deg/s)\n",thetaA_dot*180/M_PI);
	printf(" psi_dot   =   %3.3f\t(deg/s)\n",psiA_dot*180/M_PI);
}