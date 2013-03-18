import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;


public class Utilities {
	public static void PrintVector(Double[] v) {
		for(int j=0; j<v.length; j++) 
			System.out.print(v[j] + " ");
		System.out.print("\n");
	}

	public static void PrintMatrix(Double[][] v) {
		for(int i=0; i<v.length; i++)
			PrintVector(v[i]);
	}
	
	public static Double[][] ReadData(String filename) throws Exception {
		Double data[][] = null;
		List<String> datalist = new ArrayList<String>();
		
		BufferedReader datafile = new BufferedReader( new FileReader(filename) );
        String line; while ( (line = datafile.readLine())!=null ) datalist.add(line);
                
        int N = datalist.size();
        int MplusClass = datalist.get(0).split(",").length;
        
        data = new Double[N][MplusClass];
        int n=0;
        for(String instance : datalist) {
        	String[] num_strings = instance.split(",");
        	for(int m=0; m<MplusClass; m++) 
        		data[n][m] = Double.parseDouble(num_strings[m]);
        	n++;
        }
		
		return data;
	}
	
	public static Double Accuracy(Classifier c, Double[][] testdata) {
		int N = testdata.length;
		int M = testdata[0].length-1;
		
		int successes = 0;
		for(int n=0; n<N; n++) {
			Double res = c.Classify(testdata[n]);
			if(res.equals(testdata[n][M])) 
				successes++;
		}
		
		return ((double)successes)/N;
	}
	
	public static Double AccuracyCV(Classifier c, Double[][] data, int numfolds) {
		
		int N = data.length;
		
		int foldsize = (int)Math.round( ((double)N)/numfolds ); 
		if(foldsize == 0) foldsize = 1;
		
		System.out.println("\nRunning a CrossValidation with foldsize=" + foldsize);
		
		Double[][] traindata = new Double[(numfolds-1)*foldsize][], 
				   testdata  = new Double[foldsize][];
		
		int count=0;
		Double accuracysum = 0.0;
		
		int bound = Math.min(N, numfolds*foldsize);
		
		for(int fold=0; fold<numfolds; fold++) {
			System.out.println("\nfold " + fold);
			for(int n=0, tn=0; n<bound; n++) {
				if ( n>=fold*foldsize && n<(fold+1)*foldsize ) {
					//System.out.print("te"+n+",");
					testdata[n-fold*foldsize] = data[n];
				}
				else {
					//System.out.print("tr"+tn+",");
					traindata[tn++] = data[n];
				}
			}
			
			c.ClearClassifier();
			c.BuildClassifier(traindata);
			
			Double accuracy = Accuracy(c,testdata);
			System.out.println("fold "+ fold + ", accuracy = " + accuracy);
			accuracysum += accuracy;
			count++;
			
		}
			
		return accuracysum/count;
	}

}
