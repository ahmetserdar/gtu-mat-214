/*
 * This version is a spin-off from version 6.
 * 
 * Inspired by the Lambda Expression Utilization in :
 * http://tedfelix.com/software/c++-callbacks.html
 * 
 * */

#include<iostream>
#include<cstdio>
#include<cassert>
#include<vector>
#include<memory>

#include "diode_circuit_exp.hh"
#include "diode_circuit_log.hh"

#include "bisection_internals.hh"
#include "bisection.hh"

#include "callback_diode.hh"

int
main
(void)
{
  using namespace std;
  
  using namespace userdefn;
  using namespace nummethods;
  
  // variables for the diode circuit
  double R   = 1e3;
  double IS  = 3e-15;
  double VIN = 3.3;
  double VT  = 25e-3;
  
  vector< shared_ptr<DiodeCircuit> > vec_cback;
  
  // polymorphic pointers declared and initialized
  vec_cback.push_back( shared_ptr<DiodeCircuit>( new DiodeCircuitWithExp( R , IS , VIN , VT ) ) );
  vec_cback.push_back( shared_ptr<DiodeCircuit>( new DiodeCircuitWithLog( R , IS , VIN , VT ) ) );
  
  // choose 0 or 1
  unsigned int index = 1;
  shared_ptr<DiodeCircuit> current = vec_cback[index];
  assert( current.get() == vec_cback[index].get() );
  
  // initialize solver
  BisectionMethod B
    ( 
      [&current](double number) { return current->callback(number); } ,
//       callback_diode , static_cast<void*>(d_log) , 
      0.68 , 1.0 , 1e-30 , 1e-3 , 1e-5 
    );
    
  // compute
  B.compute();
  
  // report
  if ( -1 != B.getFlag() )
  {
    const std::list<SolnItem> &llist = B.getList();
    int cnt;
    
    printf("\nValues and Function Values\n\n");
    cnt = 0;
    for 
      (
        std::list<SolnItem>::const_iterator 
          it = llist.begin() ; 
        it != llist.end() ; 
        ++it
      )
    {
      cnt++;
      printf
        ( "Iter %d : %.16e %.16e\n" , cnt , 
          (*it).point.val , (*it).point.fval
        );
    }
    
    printf("\nErrors in Values and Function Values\n\n");
    cnt = 0;
    for 
      (
        std::list<SolnItem>::const_iterator 
          it = llist.begin() ; 
        it != llist.end() ; 
        ++it
      )
    {
      cnt++;
      printf
        ( "Iter %d : %.16e %.16e\n" , cnt , 
          (*it).err_x , (*it).err_f
        );
    }
  }
    
  // destroy allocated space
//   delete d_exp;
//   delete d_log;
  
  return 0;
}