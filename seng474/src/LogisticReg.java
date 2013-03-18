public class LogisticReg {
	Double [][] data;
	Double [][] x;
	int N, M;
	Double[] y;
	Double[] w;
	int k = 2;
	
	Double eta = 2.0; //called kappa in the lecture

	Double threshold = 0.5; //Default threshold for classification

	public void BuildClassifier(Double [][] data) {
		this.data = data;
		this.N=data.length; //number of rows
		this.M=data[0].length-1; //number of attributes (the last attribute is attribute y)

		//Fill in x
		int n;
		this.x = new Double[N][M];
		for(n=0; n<N && data[n]!=null; n++)
			for(int m=0; m<M; m++)
				x[n][m] = data[n][m];
		N=n;

		this.y = new Double[N];
		for(n=0; n<N; n++) y[n] = data[n][M];

		//Now do the computation.
		this.w = ComputeWeightsWithGD();
	}

	public void ClearClassifier() {
		this.w = null;
	}

	public Double Classify(Double[] xn) {
		Double p;
		Double wx = (double) 0;
		for (int i = 0;i < xn.length;i++) {
			wx += xn[i]*w[i];
		}
		p = 1 / (1 + Math.exp(-1) * wx);
		if (p >= 0.5) return (double) 1;
		else return (double) -1;
	}

	Double[] Gradient(Double[] w) {
		Double[] g = new Double[M];
		for (int i = 0;i < M;i++) g[i] = (double) 0;
		for (int i = 0;i < N;i++) {
			Double wxi = (double) 0;
			for (int j = 0;j < M;j++) {
				wxi += (w[j] * x[i][j]);
			}
			for (int j = 0;j < M;j++) {
				g[j] += y[i] * x[i][j] / (1 + Math.exp(y[i]*wxi));
			}
		}
		for (int j = 0;j < g.length;j++) {
			g[j] = g[j]/(-1 * N);
		}
		return g;
	}

	Double E(Double[] w) {
		Double output = (double)0;
		for (int i = 0;i < N;i++) {
			Double wxi = (double) 0; 
			for (int j = 0;j < M;j++) {
				wxi += (w[j] * x[i][j]);
			}
			output += Math.log(1 + Math.exp((-1)*y[i]*wxi));
		}
		return (output/N);
	}
	
	Double[] ComputeWeightsWithGD() {
		this.w = new Double[M];
		for (int i = 0;i < w.length;i++) w[i] = (double) 0;
		for (int i = 0;i < 50;i++) {
			Double[] g = Gradient(w);
			Double e = E(w);
			for (int j = 0;j < w.length;j++) {
				w[j] = w[j] - k * g[j];
			}
		}
		return w;
	}

	public Double[] getW() {
		return w;
	}

	//Shouldn't be used as w's are computed internally, unless for testing an external w vector.
	public void setW(Double[] w) {
		this.w = w;
	}

	public Double getEta() {
		return eta;
	}

	public void setEta(Double eta) {
		this.eta = eta;
	}

	public Double getThreshold() {
		return threshold;
	}

	public void setThreshold(Double threshold) {
		this.threshold = threshold;
	}


	public static void main(String[] args) throws Exception {

		Double [][] data = {
				{1.0,	1.0,	1.0,	1.0},
				{0.9,	1.0,	1.0,	1.0},
				{0.9,	0.875,	1.0,	1.0},
				{0.7,	0.75,	1.0,	-1.0},
				{0.6,	0.875,	1.0,	-1.0},
				{0.6,	0.875,	1.0,	1.0},
				{0.5,	0.75,	1.0,	-1.0},
				{0.5,	0.8125,	1.0,	-1.0},
				{0.5,	1.0,	1.0,	1.0},
				{0.5,	0.875,	1.0,	-1.0},
				{0.5,	0.875,	1.0,	1.0}
		};

		LogisticReg lr = new LogisticReg();

		lr.BuildClassifier(data);

		for (double d : lr.w) System.out.println(d);
	}

}
