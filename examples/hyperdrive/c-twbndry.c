/*BHEADER**********************************************************************
 * Copyright (c) 2013, Lawrence Livermore National Security, LLC. 
 * Produced at the Lawrence Livermore National Laboratory. Written by 
 * Jacob Schroder schroder2@llnl.gov, Rob Falgout falgout2@llnl.gov,
 * Tzanio Kolev kolev1@llnl.gov, Ulrike Yang yang11@llnl.gov, 
 * Veselin Dobrev dobrev1@llnl.gov, et al. 
 * LLNL-CODE-660355. All rights reserved.
 * 
 * This file is part of XBraid. Email schroder2@llnl.gov on how to download. 
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License (as published by the Free Software
 * Foundation) version 2.1 dated February 1999.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the terms and conditions of the GNU General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 ***********************************************************************EHEADER*/


#include <math.h>
#include "advect_data.h"
void
twbndry1( double *bdata0, double *bdata1, int s, double t, double dt, advection_setup *kd_ )
{
   const double pi=M_PI;
   double dt2 = dt*dt, dt3 = dt*dt*dt;
   double x0 = 0;
   double x1 = kd_->L;
#define bcnr(i) compute_index_1d(kd_->bcnr_, i)   
   
   if (bcnr(1)==0)
   { /* periodic case does not need any boundary data */
      *bdata0 = 0.0;
      *bdata1 = 0.0;      
   }
   else
   { /* non-periodic case */
      if( kd_->pnr == 1 )
      {
         if (s==1)
         {/*  g */
            *bdata0 = sin(pi*x0*x0+kd_->ph)*cos(t);
            *bdata1 = sin(pi*x1*x1+kd_->ph)*cos(t);
         }
         else if( s == 2 )
         { /* g + dt/2*g' */
            *bdata0 = sin(pi*x0*x0+kd_->ph)*cos(t)-dt*sin(pi*x0*x0+kd_->ph)*sin(t)/2;
            *bdata1 = sin(pi*x1*x1+kd_->ph)*cos(t)-dt*sin(pi*x1*x1+kd_->ph)*sin(t)/2;
         }
         else if( s == 3 )
         { /* g + dt/2*g' + dt*dt/4*g'' */
            *bdata0 = sin(pi*x0*x0+kd_->ph)*cos(t)-dt*sin(pi*x0*x0+kd_->ph)*sin(t)/2 - dt2*sin(pi*x0*x0+kd_->ph)*cos(t)/4;
            *bdata1 = sin(pi*x1*x1+kd_->ph)*cos(t)-dt*sin(pi*x1*x1+kd_->ph)*sin(t)/2 - dt2*sin(pi*x1*x1+kd_->ph)*cos(t)/4;
         }
         else if( s == 4 )
         { /* g + dt*g' + dt*dt/2*g'' + dt*dt*dt/4*g''' */
            *bdata0 = sin(pi*x0*x0+kd_->ph)*cos(t)-dt*sin(pi*x0*x0+kd_->ph)*sin(t) - dt2*sin(pi*x0*x0+kd_->ph)*cos(t)/2 +
               dt3*sin(pi*x0*x0+kd_->ph)*sin(t)/4;
            *bdata1 = sin(pi*x1*x1+kd_->ph)*cos(t)-dt*sin(pi*x1*x1+kd_->ph)*sin(t) - dt2*sin(pi*x1*x1+kd_->ph)*cos(t)/2 +
               dt3*sin(pi*x1*x1+kd_->ph)*sin(t)/4;
         }
      }
      else if ( kd_->pnr == 2 )
      {/*  w(1,i) = sin(om*(x0-t)+ph), w(2,i) = cos(om*(x0+t)) */
         if (s==1)
         {/*  g */
            *bdata0 = sin(kd_->om*(x0-t)+kd_->ph);
            *bdata1 = sin(kd_->om*(x1-t)+kd_->ph);
         }
         else if (s == 2)
         {/* g + dt/2*g' */
            *bdata0 = sin(kd_->om*(x0-t)+kd_->ph) - 0.5*dt*kd_->om*cos(kd_->om*(x0-t)+kd_->ph);
            *bdata1 = sin(kd_->om*(x1-t)+kd_->ph) - 0.5*dt*kd_->om*cos(kd_->om*(x1-t)+kd_->ph);
         }
         else if (s == 3)
         { /* g + dt/2*g' + dt*dt/4*g'' */
            *bdata0 = sin(kd_->om*(x0-t)+kd_->ph) - 0.5*dt*kd_->om*cos(kd_->om*(x0-t)+kd_->ph) - 0.25*dt*dt*kd_->om*kd_->om*sin(kd_->om*(x0-t)+kd_->ph);
            *bdata1 = sin(kd_->om*(x1-t)+kd_->ph) - 0.5*dt*kd_->om*cos(kd_->om*(x1-t)+kd_->ph) - 0.25*dt*dt*kd_->om*kd_->om*sin(kd_->om*(x1-t)+kd_->ph);
         }
         else if (s == 4)
         { /* g + dt*g' + dt*dt/2*g'' + dt*dt*dt/4*g''' */
            *bdata0 = sin(kd_->om*(x0-t)+kd_->ph) - dt*kd_->om*cos(kd_->om*(x0-t)+kd_->ph) - 0.5*dt*dt*kd_->om*kd_->om*sin(kd_->om*(x0-t)+kd_->ph) + 
               0.25*dt*dt*dt*kd_->om*kd_->om*kd_->om*cos(kd_->om*(x0-t)+kd_->ph);
            *bdata1 = sin(kd_->om*(x1-t)+kd_->ph) - dt*kd_->om*cos(kd_->om*(x1-t)+kd_->ph) - 0.5*dt*dt*kd_->om*kd_->om*sin(kd_->om*(x1-t)+kd_->ph) + 
               0.25*dt*dt*dt*kd_->om*kd_->om*kd_->om*cos(kd_->om*(x1-t)+kd_->ph);
         }
      }
   }
#undef bcnr
}
