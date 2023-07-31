// Structure to store gauss values of all peaks
struct gaussValues
{
	std::vector<Double_t> amplitudes;
	std::vector<Double_t> means;
	std::vector<Double_t> sigmas;

	std::vector<Double_t> meanErrors;
	std::vector<Double_t> sigmaErrors;

	std::vector<Double_t> tailAmplitudes;
	std::vector<Double_t> tailBetas;
};

// Parameters of fit
struct fitParams
{
	Int_t numPeaks;
	Double_t rangeMin;
	Double_t rangeMax;
	Double_t backgroundSlope;
	Double_t backgroundOffset;
	gaussValues initialValues;
};

// ADC to eV linear function parameters
struct calibrationSettings
{
	Int_t sdd_id;
	Double_t slope;
	Double_t intercept;
	TGraph *graph;
};