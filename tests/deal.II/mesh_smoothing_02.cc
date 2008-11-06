//----------------------------  mesh_smoothing_02.cc  ---------------------------
//    $Id$
//    Version: $Name$ 
//
//    Copyright (C) 2006, 2007, 2008 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  mesh_smoothing_02.cc  ---------------------------


// a redux of mesh_smoothing_01: when we have patch_level_1 set, and if we
// have a mesh like this:
//
// *-*-*-*-*---*---*
// *-*-*-*-*   |   |
// *-*-*-*-*---*---*
// *-*-*-*-*   *   |
// *-*-*-*-*---*---*
// |   |   |   |   |
// *---*---*---*---*
// |   |   |   |   |
// *---*---*---*---*
//
// Note that this is a patch-level-1 mesh. Now, if we set coarsen flags on all
// cells, then the result is no longer a patch-level-1
// mesh. Triangulation::prepare_coarsen_and_refinement didn't see this coming,
// and we violate the invariant that we hit again the next time we call this
// function


char logname[] = "mesh_smoothing_02/output";


#include "../tests.h"


#include <base/logstream.h>
#include <grid/tria.h>
#include <grid/grid_generator.h>
#include <grid/tria_accessor.h>
#include <grid/tria_iterator.h>

#include <fstream>
#include <iostream>


using namespace dealii;



template <int dim>
bool cell_is_patch_level_1 (const typename Triangulation<dim>::cell_iterator &cell)
{
  Assert (cell->active() == false, ExcInternalError());

  unsigned int n_active_children = 0;
  for (unsigned int i=0; i<cell->n_children(); ++i)
    if (cell->child(i)->active())
      ++n_active_children;

  return (n_active_children == 0) || (n_active_children == cell->n_children());
}


void test ()
{
  Triangulation<2> triangulation (Triangulation<2>::maximum_smoothing);
  GridGenerator::hyper_cube (triangulation);
  triangulation.refine_global (2);

				   // refine the offspring of one of the cells
				   // on level 1
  for (unsigned int i=0; i<4; ++i)
    triangulation.begin(1)->child(i)->set_refine_flag ();
  triangulation.execute_coarsening_and_refinement ();

  deallog << "n_active_cells = " << triangulation.n_active_cells()
	  << std::endl;


  for (Triangulation<2>::cell_iterator
	 cell = triangulation.begin();
       cell != triangulation.end(); ++cell)
    {
      deallog << "Cell = " << cell
	      << (cell->active() ? " is active " : " is not active ");
      if (!cell->active())
	{
	  deallog << "and has children: ";
	  for (unsigned int i=0; i<4; ++i)
	    deallog << cell->child(i) << ' ';
	}
      deallog << std::endl;
    }
  
				       // now flag everything for coarsening
				       // again
  for (Triangulation<2>::active_cell_iterator
	 cell = triangulation.begin_active();
       cell != triangulation.end(); ++cell)
    cell->set_coarsen_flag ();
  triangulation.execute_coarsening_and_refinement ();

  deallog << "n_active_cells = " << triangulation.n_active_cells()
	  << std::endl;
  
  for (Triangulation<2>::cell_iterator
	 cell = triangulation.begin();
       cell != triangulation.end(); ++cell)
    {
      Assert ((cell->refine_flag_set() == false)
	      &&
	      (cell->coarsen_flag_set() == false),
	      ExcInternalError());
      if (!cell->active())
	Assert (cell_is_patch_level_1<2>(cell),
		ExcInternalError());
    }

  deallog << "OK" << std::endl;
}



int main () 
{
  std::ofstream logfile(logname);
  logfile.precision (3);
  
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);
  try
    {
      test();
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      std::cerr << "Exception on processing: " << std::endl
		<< exc.what() << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;

      return 1;
    }
  catch (...) 
    {
      std::cerr << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      std::cerr << "Unknown exception!" << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      return 1;
    }

  return 0;
}
