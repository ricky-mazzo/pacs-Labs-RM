OBJS =  matrix.o
TEST_OBJS = test_matrix_mult.o
TESTS = test_matrix_mult
HEADERS = matrix.h

.PHONY : tests

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

test_matrix_mult : test_matrix_mult.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@  $< -lmatrix $(LIBS)

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(TEST_OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS) $(TEST_OBJS)

distclean : clean
	$(RM) $(TESTS) \
	libmatrix.a 
