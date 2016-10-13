#include "TH1.h"
#include "TMath.h"
#include "iostream"

void getSigma(){

        double mt_values[] = { 
        0.935,
        0.871,
        0.999,
        1.052,
        1.377,
        2.198,
        1.187,
        1.800,
        0.977
        };

        double mm_values[] = {
        0.948,
        1.051,
        1.000,
        1.011,
        1.210,
        1.415,
        1.294,
        1.810,
        1.343
        };

        double mt_errors[] = {
        0.029,
        0.046,
        0.046,
        0.074,
        0.112,
        0.375,
        0.152,
        1.411,
        0.164
        };

        double mm_errors[] = {
        0.005,
        0.008,
        0.008,
        0.014,
        0.020,
        0.058,
        0.021,
        0.142,
        0.024,
        };
	int ncat=9;

	for(int i=0;i<ncat;i++){

            double diff = TMath::Abs(mm_values[i]-mt_values[i]);
            double err  = TMath::Sqrt(TMath::Power(mm_errors[i],2)+TMath::Power(mt_errors[i],2));

            if(err<diff) cout << diff << " " << err << " >1 sigma " << diff/err <<  endl;
            else cout << diff << " " << err << " <=1 sigma " << diff/err << endl;
	}
}
