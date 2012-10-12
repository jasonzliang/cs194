# include "sparselib.test.hh"

# define LOOP(N)   for ( unsigned i = 0 ; i < N ; ++i ) 
# define REPEAT(N) for ( unsigned iii = 0 ; iii < N ; ++iii )

typedef double Real ;

# if defined(__dest_os) && ((__dest_os == __win32_os) || (__dest_os == __mac_os) || (__dest_os == __mac_os_x))
  # include <ctime>
  using std::clock ;
  static Real o_time = 0 ;

  static
  Real Elapsed() {
    Real n_time = clock() ;
    Real e = (n_time-o_time)/CLOCKS_PER_SEC ;
    o_time = n_time ;
    return e ;
  }
# else
  # include <sys/time.h>
  # include <sys/resource.h>
  static Real o_sec = 0 ;

  static
  Real Elapsed() {
    struct rusage ru;
    getrusage(RUSAGE_SELF,&ru);
    Real sec = ru.ru_utime.tv_sec + 1e-6 * ru.ru_utime.tv_usec ;
    Real e = sec - o_sec ;
    o_sec = sec ;
    return e ;
  }
# endif


static Real timea, timeb ;

static void out(unsigned const   nop,
                Real     const & ta,
                Real     const & tb) {
  cout << " TIME:"
       << setw(10) << ta
       << ":"
       << setw(10) << tb
       << " -- FP:"
       << setw(10) << 1e-6*nop/ta 
       << ":"
       << setw(10) << 1e-6*nop/tb 
       << endl ;
}

// test CRow  ********************************************************

static
void 
test_CRow(unsigned N,
          CRowMatrix<Real> const & crow,
          Vector<Real>     const & v) {

  Vector<Real> res( v . size() ) ;

  Elapsed() ;
  REPEAT(N) res = crow * v ;
  timea = Elapsed() ;

  REPEAT(N) {
    unsigned const * R = crow . getR() ;
    unsigned const * J = crow . getJ() ;
    Real     const * A = crow . getA() ;

    for ( unsigned i = 0 ; i < crow . nrows() ; ++i ) {
      Real tmp = 0 ;
      for ( unsigned j = R[i] ; j < R[i+1] ; ++j )
        tmp -= A[j] * v[ J[j] ] ;
      res[i] = tmp ;
    }
  }
  
  timeb = Elapsed() ;

  cout << "CRowMatrix:  "  ;
  out(N * crow . nnz(), timea, timeb) ;

}

// test CCol ********************************************************

static
void
test_CCol(unsigned N,
          CColMatrix<Real> const & ccol,
          Vector<Real>     const & v) {

  Vector<Real> res( v . size() ) ;

  Elapsed() ;
  REPEAT(N) res = ccol * v ;
  timea = Elapsed() ;

  REPEAT(N) {
    unsigned const * I = ccol . getI() ;
    unsigned const * C = ccol . getC() ;
    Real     const * A = ccol . getA() ;

    res = 0 ;
    for ( unsigned i = 0 ; i < ccol . ncols() ; ++i ) {
      Real tmp = v[i] ;
      for ( unsigned j = C[i] ; j < C[i+1] ; ++j )
        res[ I[j] ] += A[j] * tmp ;
    }
  }
  timeb = Elapsed() ;

  cout << "CColMatrix:  " ;
  out(N * ccol . nnz(), timea, timeb) ;
}

// test CCoor ********************************************************
static
void
test_CCoor(unsigned N,
           CCoorMatrix<Real> const & ccoor,
           Vector<Real>      const & v) {

  Vector<Real> res( v . size() ) ;

  Elapsed() ;
  REPEAT(N) res = ccoor * v ;
  timea = Elapsed() ;
  
  REPEAT(N) {
    unsigned const * I = ccoor . getI() ;
    unsigned const * J = ccoor . getJ() ;
    Real     const * A = ccoor . getA() ;
    res = 0 ;
    for ( unsigned i = 0 ; i < ccoor . nnz() ; ++i ) {
      res[ I[i] ] += A[i] * v[ J[i] ] ;
    }
  }
  timeb = Elapsed() ;

  cout << "CCoorMatrix: " ;
  out(N * ccoor . nnz(), timea, timeb) ;
}

void test_timing() ;

void test_timing() {

  unsigned const n      = 10000 ;
  unsigned const max_nz = 110000 ;

  Real scal = 1.23 ;
  Vector<Real> a(n), b(n), res(n), res1(n) ;

  LOOP(n) {
    a[i] = ((i+1)%7)  * 2.134 + 1 ;
    b[i] = ((i+2)%31) * 3.234 + 1 ;
  }

  res = a + b ;
  LOOP(n) res1[i] = a[i] + b[i] ;
  cout << "test vector + vector " << dist(res,res1) << endl ;

  res = a - b ;
  LOOP(n) res1[i] = a[i] - b[i] ;
  cout << "test vector - vector " << dist(res,res1) << endl ;

  res = a * b ;
  LOOP(n) res1[i] = a[i] * b[i] ;
  cout << "test vector * vector " << dist(res,res1) << endl ;

  res = a / b ;
  LOOP(n) res1[i] = a[i] / b[i] ;
  cout << "test vector / vector " << dist(res,res1) << endl ;

  res = scal + a ;
  LOOP(n) res1[i] = scal + a[i] ;
  cout << "test scal + vector " << dist(res,res1) << endl ;

  res = scal - a ;
  LOOP(n) res1[i] = scal - a[i] ;
  cout << "test scal - vector " << dist(res,res1) << endl ;
  
  res = scal * a ;
  LOOP(n) res1[i] = scal * a[i] ;
  cout << "test scal * vector " << dist(res,res1) << endl ;

  res = a + scal * b ;
  LOOP(n) res1[i] = a[i] + scal * b[i] ;
  cout << "test vector + scal * vector " << dist(res,res1) << endl ;

  res = a - scal * b ;
  LOOP(n) res1[i] = a[i] - scal * b[i] ;
  cout << "test vector + scal * vector " << dist(res,res1) << endl ;
 
  cout << endl
       << "MATRIX TESTS" 
       << endl ;

  CCoorMatrix<Real> ccoor(n, n, max_nz) ;
  CRowMatrix<Real>  crow ;
  CColMatrix<Real>  ccol ;

  cout << "build..." ;
  cout . flush() ;

  for ( unsigned i = 0 ; i < n ; ++i ) {
    for ( int j = -5 ; j <= 5 ; ++j ) {
      Real tmp = i+j;
      int jj = static_cast<int>(i + n * j / 20) ;
      if ( jj >= 0 && jj < static_cast<int>(ccoor . ncols()) )
        ccoor . insert(i, static_cast<unsigned>(jj) ) = tmp ;
    }
  }

  cout << "done" << endl ;

  ccoor . internal_order() ;

  Elapsed() ;
  crow = ccoor ;

  timea = Elapsed() ;
  ccol = ccoor ;
  
  timeb = Elapsed() ;
  
  cout << "TO CROW " << timea << "s" << endl ;
  cout << "TO CCOL " << timeb << "s" << endl ;

  Vector<Real> v(n) ;

  v = 1 ;
  v[n/2] = 234 ;
  v[n/4] = -3 ;
  v[(n*3)/4 ] = -12 ;

  //cout << "CCOOR= " << ccoor << endl ;
  //cout << "CROW= " << crow  << endl ;
  //cout << "CCOL= " << ccol  << endl ;

  res = ccoor * v ;
  res = res - crow * v ;
  cout << "CRow err = " << normi(res) << endl ;

  res = ccoor * v ;
  res = res - ccol * v ;
  cout << "CCol err = " << normi(res) << endl ;
  
  unsigned cicle_repeat = 400 ;
  test_CRow (cicle_repeat, crow,  v) ;
  test_CCol (cicle_repeat, ccol,  v) ;
  test_CCoor(cicle_repeat, ccoor, v) ;

  cout << endl << "FINAL CHECK" << endl << endl ;

  // check operation
  Real s = 2.3 ;
  
  res = ccoor * v           ; cout << "0-" << flush ;
  res = s * (ccoor * v)     ; cout << "1-" << flush ;
  res = v + ccoor * v       ; cout << "2-" << flush ;
  res = v - ccoor * v       ; cout << "3-" << flush ;
  res = v + s * (ccoor * v) ; cout << "4-" << flush ;

  res = crow * v            ; cout << "5-" << flush ;
  res = s * (crow * v)      ; cout << "6-" << flush ;
  res = v + crow * v        ; cout << "7-" << flush ;
  res = v - crow * v        ; cout << "8-" << flush ;
  res = v + s * (crow * v)  ; cout << "9-" << flush ;

  res = ccol * v            ; cout << "10-" << flush ;
  res = s * (ccol * v)      ; cout << "11-" << flush ;
  res = v + ccol * v        ; cout << "12-" << flush ;
  res = v - ccol * v        ; cout << "13-" << flush ;
  res = v + s * (ccol * v)  ; cout << "14"  << endl ;
  
  cout << endl << "TEST_TIME ALL DONE" << endl << endl ;

}
