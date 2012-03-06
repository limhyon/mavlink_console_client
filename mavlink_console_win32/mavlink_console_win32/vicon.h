#ifndef _VICON_H_
#define _VICON_H_

void initialize_VICON( void );
void RK_integration( double* q1 , double* q1_ , double* q2 , double* q2_ , double* sen , double* sen_ , double wn , double zeta );
void retrieve_VICON( void );
void view_data( void );

#endif