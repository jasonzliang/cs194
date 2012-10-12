# include "sparselib.test.hh"

class all_vector_test {

  Vector<value_t> a, b, c, res, rres ;
  value_t         scalar ;

  template <typename T>
  void
  test_diff(Vector<T> const & x, Vector<T> const & y) {
    value_t err = 0 ;
    for ( index_type i = 0 ; i < x . size() ; ++i ) {
      value_t bf = x[i] - y[i] ;
      err += bf > 0 ? bf : -bf ;
    }
    test_diff(err) ;
  }

  template <typename T>
  void
  test_diff(T const & err) {
    if ( err < 1e-10 ) {
      cout << " OK! " << err << endl ;
    } else {
      cout << " ************** ERR = " << err << endl ;
      for ( index_type i = 0 ; i < res . size() ; ++i )
        cout << i << " err = "
             << setw(15) << setprecision(10) << res[i] - rres[i] << " "
             << setw(15) << setprecision(10) << res[i] << " "
             << setw(15) << setprecision(10) << rres[i] << endl ;
      exit(0) ;
    }
  }

public:

  all_vector_test(index_type const n) : a(n), b(n), c(n), res(n), rres(n) { }
  ~all_vector_test() {}
  
  void preco(void) {
    index_type i ;
    for ( i = 0 ; i < a.size() ; ++i ) a[i] = 1.2+i*sqrt(2.0) ;
    for ( i = 0 ; i < b.size() ; ++i ) b[i] = 1+i*sqrt(3.0) ;
    c = a ;
    scalar = sqrt(2.0) ;
  }
  
  void do_all_tests(void) {
    cout << "*******************************" << endl 
         << "*******************************" << endl ;

    test001() ; test002() ; test003() ; test004() ; test005() ;
    test006() ; test007() ; test008() ; test009() ; test010() ;
    
    test011() ; test012() ; test013() ; test014() ; test015() ;
    test016() ; test017() ; test018() ; test019() ; test020() ;
    
    test021() ; test022() ; test023() ; test024() ; test025() ;
    test026() ; test027() ; test028() ; test029() ; test030() ;
    
    test031() ; test032() ; test033() ; test034() ; test035() ;
    test036() ; test037() ; test038() ; test039() ; test030() ;
    
    test041() ; test042() ; test043() ; test044() ; test045() ;
    test046() ; test047() ; test048() ; test049() ; test050() ;
 
    test051() ; test052() ; test053() ; test054() ; test055() ;
    test056() ; test057() ; test058() ; test059() ; test060() ;

    test061() ; test062() ;
    
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
    
  void test011() ;
  void test012() ;
  void test013() ;
  void test014() ;
  void test015() ;
  void test016() ;
  void test017() ;
  void test018() ;
  void test019() ;
  void test020() ;
    
  void test021() ;
  void test022() ;
  void test023() ;
  void test024() ;
  void test025() ;
  void test026() ;
  void test027() ;
  void test028() ;
  void test029() ;
  void test030() ;
    
  void test031() ;
  void test032() ;
  void test033() ;
  void test034() ;
  void test035() ;
  void test036() ;
  void test037() ;
  void test038() ;
  void test039() ;
  void test040() ;
    
  void test041() ;
  void test042() ;
  void test043() ;
  void test044() ;
  void test045() ;
  void test046() ;
  void test047() ;
  void test048() ;
  void test049() ;
  void test050() ;
 
  void test051() ;
  void test052() ;
  void test053() ;
  void test054() ;
  void test055() ;
  void test056() ;
  void test057() ;
  void test058() ;
  void test059() ;
  void test060() ;

  void test061() ;
  void test062() ;
   
} ;

void all_vector_test::test001(void) {
  cout << "test(1)" << endl ;

  scalar = 1.2345 ;

  cout << "vector = scalar" ;
  res = scalar ;
  for ( index_type i = 0 ; i < a . size() ; ++i) rres[i] = scalar ;
  test_diff(res, rres) ;
    
  preco() ;
  cout << "vector += scalar" ;
  res += scalar ;
  for ( index_type i = 0 ; i < a . size() ; ++i) rres[i] += scalar ;
  test_diff(res, rres) ;

  cout << "vector -= scalar" ;
  res -= scalar ;
  for ( index_type i = 0 ; i < a . size() ; ++i) rres[i] -= scalar ;
  test_diff(res, rres) ;

  cout << "vector *= scalar" ;
  res *= scalar ;
  for ( index_type i = 0 ; i < a . size() ; ++i) rres[i] *= scalar ;
  test_diff(res, rres) ;
    
  cout << "vector /= scalar" ;
  res /= scalar ;
  for ( index_type i = 0 ; i < a . size() ; ++i) rres[i] /= scalar ;
  test_diff(res, rres) ;

}

# define DO_TESTS(TESTNAME, OP, OPC)                      \
  cout << "vector  = " TESTNAME ;                         \
  res = OP ;                                              \
  for ( index_type i = 0 ; i < sz ; ++i) rres[i] = OPC ;  \
  test_diff(res,rres) ;                                   \
                                                          \
  cout << "vector += " TESTNAME ;                         \
  res += OP ;                                             \
  for ( index_type i = 0 ; i < sz ; ++i) rres[i] += OPC ; \
  test_diff(res,rres) ;                                   \
                                                          \
  cout << "vector -= " TESTNAME ;                         \
  res -= OP ;                                             \
  for ( index_type i = 0 ; i < sz ; ++i) rres[i] -= OPC ; \
  test_diff(res,rres) ;                                   \
                                                          \
  cout << "vector *= " TESTNAME ;                         \
  res *= OP ;                                             \
  for ( index_type i = 0 ; i < sz ; ++i) rres[i] *= OPC ; \
  test_diff(res,rres) ;                                   \
                                                          \
  cout << "vector /= " TESTNAME ;                         \
  res /= OP ;                                             \
  for ( index_type i = 0 ; i < sz ; ++i) rres[i] /= OPC ; \
  test_diff(res,rres)

# define MIN2(A,B)   min(A.size(),B.size())
# define MIN3(A,B,C) min(min(A.size(), B.size()),C.size()) ;

void all_vector_test::test002(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(2)" << endl ;
  DO_TESTS("vector", a, a[i]) ;
}

void all_vector_test::test003(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(3)" << endl ;
  DO_TESTS("- vector", - a, - a[i]) ;
}

void all_vector_test::test004(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(4)" << endl ;
  DO_TESTS("vector + vector", a + b, a[i] + b[i]) ;
}

void all_vector_test::test005(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(5)" << endl ;
  DO_TESTS("vector - vector", a - b, a[i] - b[i]) ;
}

void all_vector_test::test006(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(6)" << endl ;
  DO_TESTS("vector * vector", a * b, a[i] * b[i]) ;
}

void all_vector_test::test007(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(7)" << endl ;
  DO_TESTS("vector / vector", a / b, a[i] / b[i]) ;
}

void all_vector_test::test008(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(8)" << endl ;
  DO_TESTS("scalar + vector", scalar + a, scalar + a[i]) ;
}

void all_vector_test::test009(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(9)" << endl ;
  DO_TESTS("scalar - vector", scalar - a, scalar - a[i]) ;
}

void all_vector_test::test010(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(10)" << endl ;
  DO_TESTS("scalar * vector", scalar * a, scalar * a[i]) ;
}
  
void all_vector_test::test011(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(11)" << endl ;
  DO_TESTS("scalar / vector", scalar / a, scalar / a[i]) ;
}

void all_vector_test::test012(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(12)" << endl ;
  DO_TESTS("vector + scalar", a + scalar, a[i] + scalar) ;
}

void all_vector_test::test013(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(13)" << endl ;
  DO_TESTS("vector - scalar", a - scalar, a[i] - scalar) ;
}
 
void all_vector_test::test014(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(14)" << endl ;
  DO_TESTS("vector * scalar", a * scalar, a[i] * scalar) ;
}

void all_vector_test::test015(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(15)" << endl ;
  DO_TESTS("vector / scalar", a / scalar, a[i] / scalar) ;
}
  
  // combined test
void all_vector_test::test016(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(16)" << endl ;
  DO_TESTS("vector + scalar + vector",
	     a + scalar + b, a[i] + scalar + b[i]) ;
}

void all_vector_test::test017(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(17)" << endl ;
  DO_TESTS("vector - scalar + vector",
	    a - scalar + b, a[i] - scalar + b[i]) ;
}
  
void all_vector_test::test018(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(18)" << endl ;
  DO_TESTS("vector * scalar + vector",
	    a * scalar + b, a[i] * scalar + b[i]) ;
}
  
void all_vector_test::test019(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(19)" << endl ;
  DO_TESTS("vector / scalar + vector",
	    a / scalar + b, a[i] / scalar + b[i]) ;
}
  //-----------------
void all_vector_test::test020(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(20)" << endl ;
  DO_TESTS("vector + scalar - vector",
	    a + scalar - b, a[i] + scalar - b[i]) ;
}

void all_vector_test::test021(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(21)" << endl ;
  DO_TESTS("vector - scalar - vector",
	    a - scalar - b, a[i] - scalar - b[i]) ;
}
  
void all_vector_test::test022(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(22)" << endl ;
  DO_TESTS("vector * scalar - vector",
	        a * scalar - b, a[i] * scalar - b[i]) ;
}
  
void all_vector_test::test023(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(23)" << endl ;
  DO_TESTS("vector / scalar - vector",
	     a / scalar - b, a[i] / scalar - b[i]) ;
}
  //-----------------
void all_vector_test::test024(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(24)" << endl ;
  DO_TESTS("vector + scalar * vector",
	     a + scalar * b, a[i] + scalar * b[i] ) ;
}

void all_vector_test::test025(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(25)" << endl ;
  DO_TESTS("vector - scalar * vector",
	     a - scalar * b, a[i] - scalar * b[i]) ;
}
  
void all_vector_test::test026(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(26)" << endl ;
  DO_TESTS("vector * scalar * vector",
	     a * scalar * b, a[i] * scalar * b[i]) ;
}
  
void all_vector_test::test027(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(27)" << endl ;
  DO_TESTS("vector / scalar * vector",
	     a / scalar * b, a[i] / scalar * b[i]) ;
}
  //-----------------
void all_vector_test::test028(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(28)" << endl ;
  DO_TESTS("vector + scalar / vector",
	     a + scalar / b, a[i] + scalar / b[i]) ;
}

void all_vector_test::test029(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(29)" << endl ;
  DO_TESTS("vector - scalar / vector",
	     a - scalar / b, a[i] - scalar / b[i]) ;
}
  
void all_vector_test::test030(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(30)" << endl ;
  DO_TESTS("vector * scalar / vector",
	     a * scalar / b, a[i] * scalar / b[i]) ;
}
  
void all_vector_test::test031(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(31)" << endl ;
  DO_TESTS("vector / scalar / vector",
	     a / scalar / b, a[i] / scalar / b[i]) ;
}

// FUNCTIONS
void all_vector_test::test032(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(32)" << endl ;
  DO_TESTS("absval( vector )", absval(a), absval(a[i]) ) ;
}
void all_vector_test::test033(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(33)" << endl ;
  DO_TESTS("sin( vector )", sin(a), sin(a[i]) ) ;
}
void all_vector_test::test034(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(34)" << endl ;
  DO_TESTS("cos( vector )", cos(a), cos(a[i]) ) ;
}
void all_vector_test::test035(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(35)" << endl ;
  DO_TESTS("tan( vector )", tan(a), tan(a[i]) ) ;
}
void all_vector_test::test036(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(36)" << endl ;
  DO_TESTS("asin( vector / (1+absval(vector)) )",
	     asin(a/(1.0+absval(a))), asin(a[i]/(1.0+absval(a[i]))) ) ;
}
void all_vector_test::test037(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(37)" << endl ;
  DO_TESTS("acos( vector / (1+absval(vector)) )",
	     acos(a/(1.0+absval(a))), acos(a[i]/(1.0+absval(a[i]))) ) ;
}
void all_vector_test::test038(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(38)" << endl ;
  DO_TESTS("atan( vector )", atan(a), atan(a[i]) ) ;
}
void all_vector_test::test039(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(39)" << endl ;
  DO_TESTS("sinh( vector )", sinh(a), sinh(a[i]) ) ;
}
void all_vector_test::test040(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(40)" << endl ;
  DO_TESTS("cosh( vector )", cosh(a), cosh(a[i]) ) ;
}
void all_vector_test::test041(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(41)" << endl ;
  DO_TESTS("tanh( vector )", tanh(a), tanh(a[i]) ) ;
}
void all_vector_test::test042(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(42)" << endl ;
  DO_TESTS("sqrt( absval(vector) )", sqrt(absval(a)), sqrt(absval(a[i])) ) ;
}
void all_vector_test::test043(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(43)" << endl ;
  DO_TESTS("ceil( vector )", ceil(a), ceil(a[i]) ) ;
}
void all_vector_test::test044(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(44)" << endl ;
  DO_TESTS("floor( vector )", floor(a), floor(a[i]) ) ;
}
void all_vector_test::test045(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(45)" << endl ;
  DO_TESTS("log( 1+absval(vector) )", log(1+absval(a)), log(1+absval(a[i])) ) ;
}
void all_vector_test::test046(void) {
  preco() ;
  index_type const sz = MIN2(a,res) ;
  cout << endl << "test(46)" << endl ;
  DO_TESTS("log10( 1+absval(vector) )", log10(1+absval(a)), log10(1+absval(a[i])) ) ;
}
void all_vector_test::test047(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(47)" << endl ;
  DO_TESTS("maxval(vector,vector)", sparselib::maxval(a,b), a[i] > b[i] ? a[i] : b[i] ) ;
}
void all_vector_test::test048(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(48)" << endl ;
  DO_TESTS("minval(vector,vector)", sparselib::minval(a,b), a[i] < b[i] ? a[i] : b[i] ) ;
}
void all_vector_test::test049(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(49)" << endl ;
  DO_TESTS("pow(1+absval(vector),1+absval(vector))",
           pow(1+absval(a),1+absval(b)), pow(1+absval(a[i]),1+absval(b[i])) ) ;
}
void all_vector_test::test050(void) {
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(50)" << endl ;
  DO_TESTS("atan2(vector,vector)", atan2(a,b), atan2(a[i],b[i]) ) ;
}
void all_vector_test::test051(void) {
  preco() ;
  cout << endl << "test(51) norm1(vector)" ; cout . flush() ;
  value_t bfa = norm1(a), bfb(0) ;
  for ( index_type i = 0 ; i < a.size() ; ++i) bfb += absval(a[i]) ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test052(void) {
  preco() ;
  cout << endl << "test(52) norm2(vector)" ; cout . flush() ;
  value_t bfa = norm2(a), bfb(0) ;
  for ( index_type i = 0 ; i < a . size() ; ++i) bfb += power2(value_t(a[i])) ;
  test_diff(absval(bfa-sqrt(bfb))) ;
}
void all_vector_test::test053(void) {
  preco() ;
  cout << endl << "test(53) normi(vector)" ; cout . flush() ;
  value_t bfa = normi(a), bfb = 0 ;
  for ( index_type i = 0 ; i < a.size() ; ++i)
    if ( bfb < absval(a[i]) ) bfb = absval(a[i]) ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test054(void) {
  preco() ;
  cout << endl << "test(54) maxval(vector)" ; cout . flush() ;
  value_t bfa = maxval(a), bfb = a[0] ;
  for ( index_type i = 0 ; i < a.size() ; ++i)
    if ( bfb < a[i] ) bfb = a[i] ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test055(void) {
  preco() ;
  cout << endl << "test(55) minval(vector)" ; cout . flush() ;
  value_t bfa = minval(a), bfb = a[0] ;
  for ( index_type i = 0 ; i < a.size() ; ++i)
    if ( bfb > a[i] ) bfb = a[i] ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test056(void) {
  preco() ;
  cout << endl << "test(56) dot(vector,vector)" ; cout . flush() ;
  value_t bfa = dot(a,b), bfb = 0 ;
  for ( index_type i = 0 ; i < MIN2(a,b) ; ++i) bfb += a[i]*b[i] ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test057(void) {
  preco() ;
  cout << endl << "test(57) dot_div(vector,1+absval(vector))" ; cout . flush() ;
  value_t bfa = dot_div(a,1+absval(b)), bfb = 0 ;
  for ( index_type i = 0 ; i < MIN2(a,b) ; ++i) bfb += a[i]/(1+absval(b[i])) ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test058(void) {
  preco() ;
  cout << endl << "test(58) dist(vector,vector)" ; cout . flush() ;
  value_t bfa = dist(a,b), bfb = 0 ;
  for ( index_type i = 0 ; i < MIN2(a,b) ; ++i) bfb += power2(a[i]-b[i]) ;
  test_diff(absval(bfa-sqrt(bfb))) ;
}
void all_vector_test::test059(void) {
  preco() ;
  cout << endl << "test(59) dist2(vector,vector)" ; cout . flush() ;
  value_t bfa = dist2(a,b), bfb = 0 ;
  for ( index_type i = 0 ; i < MIN2(a,b) ; ++i) bfb += power2(a[i]-b[i]) ;
  test_diff(absval(bfa-bfb)) ;
}
void all_vector_test::test060(void) {
  preco() ;
  cout << endl << "test(60) normp(vector,scalar)" ; cout . flush() ;
  value_t bfa = normp(a,scalar), bfb = 0 ;
  for ( index_type i = 0 ; i < a.size() ; ++i) {
    value_t bf = absval(a[i]) ;
    bfb += pow(bf,scalar) ;
  }
  test_diff( absval(bfa-pow(bfb,1/scalar)) ) ;
}
void all_vector_test::test061(void) { // specializations
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(61)" << endl ;
  DO_TESTS("vector * scalar + vector",
	    b * scalar + c, b[i] * scalar + c[i]) ;
}
void all_vector_test::test062(void) { // specializations
  preco() ;
  index_type const sz = MIN3(a,b,res) ;
  cout << endl << "test(62)" << endl ;
  DO_TESTS("vector * scalar + vector",
	    b * 2 + c, b[i] * 2 + c[i]) ;
}

//********************************************************************
//********************************************************************
//********************************************************************
//********************************************************************
void test_vector() ;

void test_vector() {
  all_vector_test allv(3) ;
  allv . do_all_tests() ;
  cout << endl << "TEST_VECTOR ALL DONE" << endl << endl ;
}
