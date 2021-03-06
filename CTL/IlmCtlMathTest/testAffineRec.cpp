///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Academy of Motion Picture Arts and Sciences
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A world-wide, royalty-free, non-exclusive right to distribute, copy,
// modify, create derivatives, and use, in source and binary forms, is
// hereby granted, subject to acceptance of this license. Performance of
// any of the aforementioned acts indicates acceptance to be bound by the
// following terms and conditions:
// 
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty.
// 
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty
//     in the documentation and/or other materials provided with the
//     distribution.
// 
//   * Nothing in this license shall be deemed to grant any rights to
//     trademarks, copyrights, patents, trade secrets or any other
//     intellectual property of A.M.P.A.S. or any contributors, except
//     as expressly stated herein, and neither the name of A.M.P.A.S.
//     nor of any other contributors to this software, may be used to
//     endorse or promote products derived from this software without
//     specific prior written permission of A.M.P.A.S. or contributor,
//     as appropriate.
// 
// This license shall be governed by the laws of the State of California,
// and subject to the jurisdiction of the courts therein.
// 
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO
// EVENT SHALL A.M.P.A.S., ANY CONTRIBUTORS OR DISTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <CtlRbfInterpolator.h>
#include <ImathVec.h>

using namespace std;

/*

The affine transformation is

| .8   .1 -.1  .2 |
| .0  1.1 -.1 -.3 |
|-.2  -.1  .7 -.1 |

*/

void
runTestAffine(int numSamples, int numTests)
{
    srand(113131);

#ifdef HAVE_DARWIN
    //  gcc on mac requires V3f to be used before it is used below
    //  (tested on g++-4.0.1)
    Imath::V3f q;
#endif

    typedef Imath::V3f V3fPair[2];
    V3fPair *p = new V3fPair[numSamples];

    for ( int s = 0; s < numSamples; s++) 
    {
	p[s][0][0] = (float) rand() / (float) RAND_MAX;
	p[s][0][1] = (float) rand() / (float) RAND_MAX;
	p[s][0][2] = (float) rand() / (float) RAND_MAX;
	
	p[s][1][0] = .8*p[s][0][0] + .1*p[s][0][1] -.1*p[s][0][2] + .2;
	p[s][1][1] = .0*p[s][0][0] +1.1*p[s][0][1] -.1*p[s][0][2] + .3;
	p[s][1][2] =-.2*p[s][0][0]  -.1*p[s][0][1] +.7*p[s][0][2]  -.1;	
    }
    
    Ctl::RbfInterpolator rbfItp(numSamples, p);

    cout << "Testing interpolation" << endl;
    double maxErr  = .0;
    double meanErr = .0;

    for ( int s = 0; s < numSamples; s++) 
    {
	double err = (rbfItp.value(p[s][0]) - p[s][1]).length();

	if (err > maxErr)
	    maxErr = err;

	meanErr += err;
    }

    assert (maxErr < 1E-6);
    assert (meanErr/double(numTests)  < 1E-6);

    delete [] p;

    cout << "Testing reconstruction" << endl;
    maxErr  = .0;
    meanErr = .0;

    V3fPair *tests = new V3fPair[numTests];

    for ( int s = 0; s < numTests; s++) 
    {	
	tests[s][0][0] = (float) rand() / (float) RAND_MAX;
	tests[s][0][1] = (float) rand() / (float) RAND_MAX;
	tests[s][0][2] = (float) rand() / (float) RAND_MAX;
	
	tests[s][1][0] = .8*tests[s][0][0] + .1*tests[s][0][1] -.1*tests[s][0][2] + .2;
	tests[s][1][1] = .0*tests[s][0][0] +1.1*tests[s][0][1] -.1*tests[s][0][2] + .3;
	tests[s][1][2] =-.2*tests[s][0][0]  -.1*tests[s][0][1] +.7*tests[s][0][2]  -.1;
	
	double err = (rbfItp.value(tests[s][0]) - tests[s][1]).length();

	if (err > maxErr)
	    maxErr = err;

	meanErr += err;
    }

    assert (maxErr < 1E-6);
    assert (meanErr/double(numTests)  < 1E-6);

    delete [] tests;
}

void
testAffineRecSmall()
{
    const int numSamples = 10;
    cout << "Testing the reconstruction of an affine function." << endl;
    cout << "With " << numSamples << " samples." << endl;
    
    try
    {
	runTestAffine(numSamples, 1000);
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
    cout << "ok" << endl;

}

void
testAffineRecLarge()
{
    const int numSamples = 1000;
    cout << "Testing the reconstruction of an affine function." << endl;
    cout << "With " << numSamples << " samples." << endl;
    
    try
    {
	runTestAffine(numSamples, 1000);
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
    cout << "ok" << endl;

}
