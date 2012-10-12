# include "sparselib.test.hh"

class all_trid_test {

  static inline value_t abs(value_t const & a) { return a > 0 ? a : -a ; }

  Vector<value_t>     a, b, RES, RRES ;
  TridMatrix<value_t> A ;
  value_t             scalar ;

  void
  test_diff(Vector<value_t> const & x, Vector<value_t> const & y) {
    value_t err = 0 ;
    for ( index_type i = 0 ; i < x.size() ; ++i ) {
      err = max(err, abs(x[i] - y[i]) ) ;
    }
    test_diff(err) ;
  }

  void
  test_diff(value_t const & err) {
    if ( err < 1e-6 ) {
      cout << " OK! " << err << endl ;
    } else {
      cout << " ************** ERR = " << err << endl ;
      for ( index_type i = 0 ; i < RES.size() ; ++i )
        cout << i << " err = "
             << setw(15) << setprecision(10) << RES[i] - RRES[i] << " "
             << setw(15) << setprecision(10) << RES[i] << " "
             << setw(15) << setprecision(10) << RRES[i] << endl ;
      exit(0) ;
    }
  }

public:

  all_trid_test(index_type const n) {
    cout << "all_trid_test_allocating..." ; cout . flush() ;
    A    . new_dim( n ) ;
    a    . new_dim( n ) ;
    b    . new_dim( n ) ;
    RES  . new_dim( n ) ;
    RRES . new_dim( n ) ;
    cout << "done" << endl ;
  }

  ~all_trid_test() { }
  
  void preco(void) {
    index_type i ;
    for ( i = 0 ; i < a.size() ; ++i ) a[i] = 1.2+i*sqrt(2.0) ;
    for ( i = 0 ; i < b.size() ; ++i ) b[i] = 1+i*sqrt(3.0) ;

    A  = a ;
    A += 3 ;
    for ( i = 1 ; i < A.nrows()-1 ; ++i )
      A(i,i-1) = A(i,i+1) = 1 ;
    scalar = sqrt(2.0) ;
  }
  
  void do_all_tests(void) {
    cout << "*******************************" << endl 
         << "*******************************" << endl ;

    test001() ;
    test002() ;
    test003() ;
    
    cout << "ALL DONE" << endl ;

  }
  
  void test001() ;
  void test002() ;
  void test003() ;
  void test004() ;
  void test005() ;
  void test006() ;
  void test007() ;
  void test008() ;
  void test009() ;
  void test010() ;

} ;

void all_trid_test::test001(void) {
  preco() ;
  cout << endl << "test(1***)" << endl ;
  index_type N(A.nrows()) ;

  RES = A * a ;
  for ( index_type i = 0 ; i < N ; ++i ) {
    RRES[i] = A(i,i) * a[i] ;
    if ( i > 0   ) RRES[i] += A(i,i-1) * a[i-1] ;
    if ( i < N-1 ) RRES[i] += A(i,i+1) * a[i+1] ;
  }
  test_diff(RES, RRES) ;
}

void all_trid_test::test002(void) {
  preco() ;
  cout << endl << "test(2)" << endl ;
  index_type N(A.nrows()) ;
  RES = b + A * a ;
  for ( index_type i = 0 ; i < N ; ++i ) {
    RRES[i] = b[i] + A(i,i) * a[i] ;
    if ( i > 0   ) RRES[i] += A(i,i-1) * a[i-1] ;
    if ( i < N-1 ) RRES[i] += A(i,i+1) * a[i+1] ;
  }
  test_diff(RES, RRES) ;
}

void all_trid_test::test003(void) {
  preco() ;
  cout << endl << "test(3)" << endl ;
  index_type N(A.nrows()) ;
  RES = b - A * a ;
  for ( index_type i = 0 ; i < N ; ++i ) {
    RRES[i] = b[i] - A(i,i) * a[i] ;
    if ( i > 0   ) RRES[i] -= A(i,i-1) * a[i-1] ;
    if ( i < N-1 ) RRES[i] -= A(i,i+1) * a[i+1] ;
  }
  test_diff(RES, RRES) ;
}

//********************************************************************
//********************************************************************
//********************************************************************
//********************************************************************
void test_trid() ;

void test_trid() {
  all_trid_test allm(4) ;
  allm . do_all_tests() ;
  cout << endl << "TEST_TRID ALL DONE" << endl << endl ;
}
