// ---------------------------------------------------------------------
//
// Copyright (C) 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// test ConsecutiveControl


#include "../tests.h"
#include <deal.II/base/utilities.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <deal.II/lac/solver_control.h>


int main(int argc, char **argv)
{
  std::ofstream logfile("output");
  logfile.precision(4);
  deallog.attach(logfile);
  deallog.threshold_double(1.e-10);

  {
    ConsecutiveControl solver_control(12345, 1.e-3, 3, false, false);
    //                                                     // n_converged_iterations:
    deallog << solver_control.check(0,1.e-1) << std::endl; // 0
    deallog << solver_control.check(1,1.e-4) << std::endl; // 1
    deallog << solver_control.check(2,1.e-5) << std::endl; // 2
    deallog << solver_control.check(3,2.e-3) << std::endl; // 0
    deallog << solver_control.check(4,5.e-4) << std::endl; // 1
    deallog << solver_control.check(5,4.e-4) << std::endl; // 2
    deallog << solver_control.check(6,3.e-4) << std::endl; // 3 ==> success

    // reuse
    deallog << solver_control.check(0,3.e-4) << std::endl; // 1
    deallog << solver_control.check(1,2.e-4) << std::endl; // 2
    deallog << solver_control.check(2,1.e-4) << std::endl; // 3 ==> success

  }
}
