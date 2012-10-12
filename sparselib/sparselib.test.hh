# include "sparselib.hh"

using namespace ::sparselib_load ;
using           ::sparselib::index_type ;
using namespace ::std ;
using ::sparselib_fun::maxval ;
using ::sparselib_fun::minval ;
using ::sparselib_fun::absval ;

typedef double value_t ;

inline
value_t power2(value_t const & a)
{ return a*a ; }

void test_vector() ;
void test_trid() ;
void test_timing();
