// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright 2014-2015 Tsinghua University
// Author: wb.th08@gmail.com (Bin Wang), ozj@tsinghua.edu.cn (Zhijian Ou) 
//
// All h, cpp, cc, and script files (e.g. bat, sh, pl, py) should include the above 
// license declaration. Different coding language may use different comment styles.


#pragma once
#include "wb-system.h"
#include <omp.h>
using namespace wb;

namespace trf
{

	typedef double PValue;
	typedef double LogP;
	typedef double Prob;
	const float INF = 1e20;
	const float LogP_zero = -INF;

	
	inline Prob LogP2Prob(LogP x) {
		return (x <= LogP_zero / 2) ? 0 : exp((double)(x));
	}
	inline LogP Prob2LogP(Prob x) {
		return ((x) <= 0) ? LogP_zero : log((double)(x));
	}
	
	inline LogP Log_Sum(LogP x, LogP y) {
		return (x > y) ? x + Prob2LogP(1 + LogP2Prob(y - x)) : y + Prob2LogP(1 + LogP2Prob(x - y));
	}
	/// log[exp(x)-exp(y)]
	inline LogP Log_Sub(LogP x, LogP y) {
		return (x > y) ? x + Prob2LogP(1 - LogP2Prob(y - x)) : y + Prob2LogP( LogP2Prob(x - y) - 1);
	}
	/// log summate all the values in array
	inline LogP Log_Sum(LogP *p, int num) {
		LogP sum = LogP_zero;
		for (int i = 0; i < num; i++) {
			sum = Log_Sum(sum, p[i]);
		}
		return sum;
	}

	template <typename T>
	T Sum(T *p, int n) {
		T sum = 0;
		for (int i = 0; i < n; i++) {
			sum += p[i];
		}
		return sum;
	}
	
	LogP LogLineNormalize(LogP* pdProbs, int nNum);
	
	int LogLineSampling(const LogP* pdProbs, int nNum);
	
	void LineNormalize(Prob* pdProbs, int nNum);
	
	int LineSampling(const Prob* pdProbs, int nNum);
	
	bool Acceptable(Prob prob);
	
	void RandomPos(int *a, int len, int n);

	/// a thread-safe version of rand()
	/// if thread_num != -1, then init the thread state
	/// if thread_num = -1, then get a random values.
	int omp_rand(int thread_num = -1);

	/// get a random float between dmin and dmax
	double dRand(double dmin, double dmax);

	/// get a random integer int [nMin, nMax-1]
	int omp_nrand(int nMin, int nMax);

	/// smooth a distribution
	void EasySmooth(Prob *p, int num, Prob threshold = 1e-5);

	/// Get the AIS intermediate factor beta_t
	/* t -> current intermediate number (from 0 to T-1); T -> total intermadiate number. */
	double GetAISFactor(int t, int T);

	/// calculate the sigmoid function f(x) = 1/(1+exp(-x))
	double SigmFunc(double x);
}
