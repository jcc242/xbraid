/*BHEADER**********************************************************************
 * Copyright (c) 2013,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of XBraid.  See file COPYRIGHT for details.
 *
 * XBraid is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 ***********************************************************************EHEADER*/

/** \file braid_test.c
 * \brief Define Braid test routines.
 */

#include "braid.h"
#include "_braid.h"
#include "util.h"

/*--------------------------------------------------------------------------
 * Some simple tests on the init, access and free routines 
 *--------------------------------------------------------------------------*/
braid_Int
braid_TestInitAccess( braid_App           app, 
                     MPI_Comm            comm_x,
                     FILE               *fp, 
                     braid_Real          t,
                     braid_PtFcnInit     init, 
                     braid_PtFcnAccess   access,
                     braid_PtFcnFree     free)
{
   
   braid_Vector    u ;
   braid_Status    status;
   braid_Int       myid_x;
   
   _braid_InitStatus( 0.0, 0, 0, 0, &status);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestInitAccess\n\n");

   /* Test */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);
   
   if(access != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   access(u) \n");
      access(app, t, status, u);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   check output: wrote u for initial condition at t=%1.2e. \n\n",t);
   }

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   free(u) \n");
   free(app, u);
   
   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestInitAccess\n");

   return 0;
}

braid_Int
braid_TestClone( braid_App        app,  
              MPI_Comm            comm_x,
              FILE               *fp, 
              braid_Real          t,
              braid_PtFcnInit     init, 
              braid_PtFcnAccess   access,
              braid_PtFcnFree     free,
              braid_PtFcnClone    clone)
{
   
   braid_Vector  u, v;
   braid_Status  status;
   braid_Int     myid_x;
   
   _braid_InitStatus( 0.0, 0, 0, 0, &status);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestClone\n\n");

   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   v = clone(u)\n");
   clone(app, u, &v);
   
   if(access != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   access(u)\n");
      access(app, t, status, u);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   access(v)\n");
      access(app, t, status, v);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   check output:  wrote u and v for initial condition at t=%1.2e.\n\n", t);

   }

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   free(v)\n");
   free(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestClone\n");
   
   return 0;
}



braid_Int
braid_TestSum( braid_App        app, 
            MPI_Comm            comm_x,
            FILE               *fp, 
            braid_Real          t,
            braid_PtFcnInit     init, 
            braid_PtFcnAccess   access,
            braid_PtFcnFree     free, 
            braid_PtFcnClone    clone,
            braid_PtFcnSum      sum )  
{
   
   braid_Vector  u, v;
   braid_Status  status;
   braid_Int     myid_x;
   
   _braid_InitStatus( 0.0, 0, 0, 0, &status);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestSum\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = u - v\n");
   sum(app, 1.0, u, -1.0, v); 

   if(access != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   access(v)\n");
      access(app, t, status, v);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   check output:  v should equal the zero vector\n\n");
   }

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   Starting Test 2\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = 2*u + v\n");
   sum(app, 2.0, u, 1.0, v); 

   if(access != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   access(v)\n");
      access(app, t, status, v);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   access(u)\n");
      access(app, t, status, u);
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   check output:  v should equal 2*u \n\n");

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   free(v)\n");
   free(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestSum\n");

   return 0;
}

braid_Int
braid_TestDot( braid_App        app, 
            MPI_Comm            comm_x,
            FILE               *fp, 
            braid_Real          t,
            braid_PtFcnInit     init, 
            braid_PtFcnFree     free, 
            braid_PtFcnClone    clone,
            braid_PtFcnSum      sum,  
            braid_PtFcnDot      dot)
{   
   braid_Vector  u, v, w;
   braid_Real    result1, result2, result3;
   braid_Int     myid_x, correct;
   double     wiggle = 1e-12;
   
   MPI_Comm_rank( comm_x, &myid_x );

   /* Initialize the flags */
   correct = 1;
   braid_Int zero_flag = 0;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestDot\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(u,u) \n");
   dot(app, u, u, &result1);
   if( fabs(result1) == 0.0)
   {
      zero_flag = 1;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Warning:  dot(u,u) = 0.0\n"); 
   }
   else if( isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Warning:  dot(u,u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(v,v) \n");
   dot(app, v, v, &result1);
   if( (fabs(result1) > wiggle) || isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 1 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 1 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   actual output:    dot(v,v) = %1.2e  \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   expected output:  dot(v,v) = 0.0 \n\n");
   

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Starting Test 2\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = u + w \n");
   sum(app, 1.0, u, 1.0, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(u,u)\n");
   dot(app, u, u, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(w,w)\n");
   dot(app, w, w, &result2);
   if( (fabs(result2/result1 - 4.0) > wiggle) || isnan(result2/result1) )
   {
      correct = 0;
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 2 Failed, Likely due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 2 Failed\n");
      }
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 2 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   actual output:    dot(w,w) / dot(u,u) = %1.2e / %1.2e = %1.2e \n",
         result2, result1, result2/result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   expected output:  dot(w,w) / dot(u,u) = 4.0 \n\n");

   /* Test 3 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Starting Test 3\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   free(w)\n");
   free(app, w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = 0.0*u + 0.5*w \n");
   sum(app, 0.0, u, 0.5, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(u,u)\n");
   dot(app, u, u, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(w,w)\n");
   dot(app, w, w, &result2);
   /* Check Result */
   if( (fabs(result2/result1 - 0.25) > wiggle) || isnan(result2/result1) )

   {
      correct = 0;
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 3 Failed, Likely due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 3 Failed\n");
      }
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 3 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   actual output:    dot(w,w) / dot(u,u) = %1.2e / %1.2e = %1.2e \n",
         result2, result1, result2/result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   expected output:  dot(w,w) / dot(u,u) = 0.25 \n\n");

   /* Test 4 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Starting Test 4\n");

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   free(w)\n");
   free(app, w);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = clone(u)\n");
   clone(app, u, &w);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   w = u + 0.5*w \n");
   sum(app, 1.0, u, 0.5, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(u,u)\n");
   dot(app, u, u, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(w,u)\n");
   dot(app, w, u, &result2);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   actual output:    dot(w,u) + dot(u,u) = %1.2e + %1.2e = %1.2e\n", 
      result2, result1, result2+result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   v = u + w \n");
   sum(app, 1.0, u, 1.0, w);   
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   dot(v,u)\n");
   dot(app, w, u, &result3);

   /* Check Result */
   if( (fabs(result2 + result1 - result3)/fabs(result3) > wiggle) || 
       isnan(result2) || isnan(result1) || isnan(result3) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 4 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   Test 4 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   actual output:    dot(v,u) = %1.2e  \n", result3);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   expected output:  dot(v,u) = dot(w,u) + dot(u,u) \n\n");
 

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   free(v)\n");
   free(app, v);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestDot:   free(w)\n");
   free(app, w);

   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestDot: all tests passed successfully\n");
   else
   {
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestDot: some tests failed, possibly due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestDot: some tests failed\n");
      }
   }

   return correct;
}

braid_Int
braid_TestBuf( braid_App           app,
            MPI_Comm               comm_x,
            FILE                   *fp, 
            braid_Real             t,
            braid_PtFcnInit        init,
            braid_PtFcnFree        free,
            braid_PtFcnSum         sum,  
            braid_PtFcnDot         dot,
            braid_PtFcnBufSize     bufsize,
            braid_PtFcnBufPack     bufpack,
            braid_PtFcnBufUnpack   bufunpack)
{   
   braid_Vector  u, v;
   braid_Real    result1;
   braid_Int     myid_x, size, correct;
   void      *buffer;
   double     wiggle = 1e-12;
   
   MPI_Comm_rank( comm_x, &myid_x );

   /* Initialize the correct flag */
   correct = 1;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestBuf\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   dot(u,u) \n");
   dot(app, u, u, &result1);
   if( fabs(result1) == 0.0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Warning:  dot(u,u) = 0.0\n"); 
   }
   else if( isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Warning:  dot(u,u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   size = bufsize()\n");
   bufsize(app, &size);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   buffer = malloc(size)\n");
   buffer = malloc(size);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   buffer = bufpack(u, buffer))\n");
   bufpack(app, u, buffer);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   v = bufunpack(buffer)\n");
   bufunpack(app, buffer, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   dot(v,v) \n");
   dot(app, v, v, &result1);
   if( (fabs(result1) > wiggle) || isnan(result1) )

   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Test 1 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Test 1 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   actual output:    dot(v,v) = %1.2e  \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   expected output:  dot(v,v) = 0.0 \n\n");
   
   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   free(v)\n");
   free(app, v);
   
   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestBuf: all tests passed successfully\n");
   else      
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestBuf: some tests failed\n");

   return correct;
}

braid_Int
braid_TestCoarsenRefine( braid_App        app,
                      MPI_Comm            comm_x,
                      FILE                *fp, 
                      braid_Real          t,
                      braid_Real          fdt,
                      braid_Real          cdt,
                      braid_PtFcnInit     init,
                      braid_PtFcnAccess   access,
                      braid_PtFcnFree     free,
                      braid_PtFcnClone    clone,
                      braid_PtFcnSum      sum,
                      braid_PtFcnDot      dot,
                      braid_PtFcnCoarsen  coarsen,
                      braid_PtFcnRefine   refine)
 {   
   braid_Vector  u, v, w, uc, vc, wc;
   braid_Real    result1;
   braid_Int     myid_x, level, correct;
   braid_Status  status;
   
   MPI_Comm_rank( comm_x, &myid_x );

   /* Initialize the correct flag */
   correct = 1;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestCoarsenRefine\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   u = init(t=%1.2e)\n", t);
   init(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   dot(u,u) \n");
   dot(app, u, u, &result1);
   if( fabs(result1) == 0.0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Warning:  dot(u,u) = 0.0\n"); 
   }
   else if( isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Warning:  dot(u,u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   uc = coarsen(u)\n");
   coarsen(app, t, t-fdt, t+fdt, t-cdt, t+cdt, u, &uc); 

   if(access != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   access(uc) \n");
      level = 1;
      _braid_InitStatus( 0.0, 0, level, 0, &status);
      access(app, t, status, uc);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   access(u) \n");
      level = 0;
      _braid_InitStatus( 0.0, 0, level, 0, &status);
      access(app, t, status, u);
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:   wrote u and spatially coarsened u \n\n");

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 2\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   vc = coarsen(v)\n");
   coarsen(app, t, t-fdt, t+fdt, t-cdt, t+cdt, v, &vc); 
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   wc = clone(vc)\n");
   clone(app, vc, &wc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   wc = uc - wc \n");
   sum(app, 1.0, uc, -1.0, wc); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   dot(wc,wc)\n");
   dot(app, wc, wc, &result1);
   
   /* We expect exact equality between uc and vc */
   if( (fabs(result1) != 0.0) || isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 2 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 2 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    dot(wc,wc) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   expected output:  dot(wc,wc) = 0.0 \n\n");

   /* Test 3 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 3\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(v)\n");
   free(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = refine(vc)\n");
   refine(app, t, t-fdt, t+fdt, t-cdt, t+cdt, vc, &v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   u = refine(uc)\n");
   refine(app, t, t-fdt, t+fdt, t-cdt, t+cdt, uc, &u); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   dot(v,v)\n");
   dot(app, v, v, &result1);
   
   /* We expect exact equality between u and v */
   if( (fabs(result1) != 0.0) || isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 3 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 3 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    dot(v,v) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   expected output:  dot(v,v) = 0.0 \n\n");

   /* Test 4 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 4\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   w = w - u \n");
   sum(app, 1.0, u, -1.0, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   dot(w,w)\n");
   dot(app, w, w, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    dot(w,w) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   %s%s%s", 
                   "expected output:  For simple interpolation formulas\n",
                   "                             (e.g., bilinear) and a known function\n",
                   "                             (e.g., constant), dot(w,w) should = 0\n\n");

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(u)\n");
   free(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(v)\n");
   free(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(w)\n");
   free(app, w);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(uc)\n");
   free(app, uc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(vc)\n");
   free(app, vc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   free(wc)\n");
   free(app, wc);


   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestCoarsenRefine: all tests passed successfully\n");
   else      
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestCoarsenRefine: some tests failed\n");
   
   return correct;
}

braid_Int
braid_TestAll( braid_App         app,
            MPI_Comm             comm_x,
            FILE                 *fp, 
            braid_Real           t,
            braid_Real           fdt,
            braid_Real           cdt,
            braid_PtFcnInit      init,
            braid_PtFcnFree      free,
            braid_PtFcnClone     clone,
            braid_PtFcnSum       sum,
            braid_PtFcnDot       dot,
            braid_PtFcnBufSize   bufsize,
            braid_PtFcnBufPack   bufpack,
            braid_PtFcnBufUnpack bufunpack,
            braid_PtFcnCoarsen   coarsen,
            braid_PtFcnRefine    refine)
{
   braid_Int    myid_x, flag = 0, correct = 1;
   
   MPI_Comm_rank( comm_x, &myid_x );
   
   /** 
    * We set the access parameter to NULL below, because this function is
    * is designed to return only one value, the boolean correct
    **/

   /* Test init(), free() */
   braid_TestInitAccess( app, comm_x, fp, t, init, NULL, free);
   braid_TestInitAccess( app, comm_x, fp, fdt, init, NULL, free);

   /* Test clone() */
   braid_TestClone( app, comm_x, fp, t, init, NULL, free, clone);
   braid_TestClone( app, comm_x, fp, fdt, init, NULL, free, clone);

   /* Test sum() */
   braid_TestSum( app, comm_x, fp, t, init, NULL, free, clone, sum);
   braid_TestSum( app, comm_x, fp, fdt, init, NULL, free, clone, sum);

   /* Test dot() */
   flag = braid_TestDot( app, comm_x, fp, t, init, free, clone, sum, dot);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestDot 1 Failed\n");
      correct = 0;
   }
   flag = braid_TestDot( app, comm_x, fp, fdt, init, free, clone, sum, dot);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestDot 2 Failed\n");
      correct = 0;
   }

   /* Test bufsize(), bufpack(), bufunpack() */
   flag = braid_TestBuf( app, comm_x, fp, t, init, free, sum, dot, bufsize, bufpack, bufunpack);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestBuf 1 Failed\n");
      correct = 0;
   }
   flag = braid_TestBuf( app, comm_x, fp, fdt, init, free, sum, dot, bufsize, bufpack, bufunpack);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestBuf 2 Failed\n");
      correct = 0;
   }
 
   /* Test coarsen and refine */
   if( (coarsen != NULL) && (refine != NULL) )
   {
      flag = braid_TestCoarsenRefine(app, comm_x, fp, t, fdt, cdt, init,
                          NULL, free, clone, sum, dot, coarsen, refine);
      if(flag == 0)
      {
         _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestCoarsenRefine 1 Failed\n");
         correct = 0;
      }
   }

   if(correct == 1)
   {
      _braid_ParFprintfFlush(fp, myid_x, "\n\n");
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestAll: all tests passed successfully\n\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "\n\n");
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestAll: some tests failed\n\n");
   }
   
   return correct;
}