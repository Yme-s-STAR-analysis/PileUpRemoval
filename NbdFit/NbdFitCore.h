#ifndef __YigeNbdFitCore__
#define __YigeNbdFitCore__

class TF1;
class TH1F;
class TH2F;

class NbdFitCore {

    private:

        TF1* fNbd;
        TH1F* h1;
        TH2F* h2;
        double x1, x2;
        double mMean, mSigma, mSkewness, mMode;

    public:

        NbdFitCore(){}
        ~NbdFitCore(){}

        void Init(TH2F* h2);
        void Init(TH2F* h2, double x1, double x2);
        void DoFit(double x1, double x2); // this x means the quantity to do the pile-up rejection, like nTofMatch/Beta_eta1, NOT RefMult3
        // float GetMean();
        // float GetSigma();
        // float GetSkewness();
        double GetMode();
        double GetLeftBoundary(double n);
        double GetRightBoundary(double n);
        TH1F* GetHist1D() { return h1; }
        TF1* GetFunction() { return fNbd; }
};


#endif