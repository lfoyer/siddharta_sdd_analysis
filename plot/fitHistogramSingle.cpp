std::vector<Double_t> findPeaks(TH1F *, Double_t, Double_t, Double_t, Double_t);
Double_t fitFunction(Double_t *, Double_t *);
Int_t numPeaks = 10;

/**
 * Perform fit on given channel according to parameters
 * @param h Histogram to fit
 * @param parameters Parameters of fit
 * @return Parameters of fitted histogram
 */
gaussValues fitHistogram(TH1F *histogram, fitParams parameters, Int_t numHist)
{
	// Find peak means
	std::vector<Double_t> highPeaks = findPeaks(histogram, 0.005, 22, parameters.rangeMin, parameters.rangeMax);
	sort(highPeaks.begin(), highPeaks.end());
	for (Double_t mean : highPeaks) {std::cout << "highpeaks: " << mean << std::endl;}

	std::vector<Double_t> lowPeaks1 = findPeaks(histogram, 0.0015, 22, highPeaks[1], highPeaks[2]);
	for (Double_t mean : lowPeaks1) {std::cout << "lowpeaks1: " << mean << std::endl;}

	/*
	std::vector<Double_t> lowPeaks2 = findPeaks(histogram, 0.002, 22, highPeaks[3]+70, highPeaks[3]+140);	
	for (Double_t mean : lowPeaks2) {std::cout << "lowpeaks2: " << mean << std::endl;}
	std::cout << highPeaks[3]+70 << " " << highPeaks[3]+130 << std::endl;

	lowPeaks1.insert(lowPeaks1.end(), lowPeaks2.begin(), lowPeaks2.end());
	*/
	
	highPeaks.insert(highPeaks.end(), lowPeaks1.begin(), lowPeaks1.end());
	parameters.initialValues.means = highPeaks;

	sort(parameters.initialValues.means.begin(), parameters.initialValues.means.end());

	for (Double_t mean : parameters.initialValues.means) {std::cout << "peakFinder: " << mean << std::endl;}
	std::cout << "numPeaks: " << numPeaks << std::endl;

	// Create fit function
	Int_t numParam = numPeaks * 3 + 2;
	TF1 *fitFunc = new TF1(Form("fitFunc%d", numHist), fitFunction, parameters.rangeMin, parameters.rangeMax, numParam);
	
	// Set initial amp, mean, and sigma parameters for fit
	for (Int_t i = 0; i < numPeaks; i++)
	{
		fitFunc->SetParameter(i * 3, parameters.initialValues.amplitudes[i]);
		fitFunc->SetParLimits(i * 3, parameters.initialValues.amplitudes[i] * 0.8, parameters.initialValues.amplitudes[i] * 1.3);
		fitFunc->SetParName(i * 3, Form("g%d: amp\t", i+1));

		fitFunc->SetParameter(i * 3 + 1, parameters.initialValues.means[i]);
		fitFunc->SetParName(i * 3 + 1, Form("g%d: mea\t", i+1));

		fitFunc->SetParameter(i * 3 + 2, parameters.initialValues.sigmas[i]);
		fitFunc->SetParLimits(i * 3 + 2, parameters.initialValues.sigmas[i] * 0.8, parameters.initialValues.sigmas[i] * 1.2);
		fitFunc->SetParName(i * 3 + 2, Form("g%d: sig\t", i+1));
	}

	// Background parameters
	fitFunc->SetParameter(numParam - 2, parameters.backgroundOffset);
	fitFunc->SetParName(numParam - 2, "offset  ");
	fitFunc->SetParameter(numParam - 1, parameters.backgroundSlope);
	fitFunc->SetParName(numParam - 1, "slope  ");

	// Perform fit
	histogram->Fit(fitFunc, "R");

	// Retrieve fitted values and add labels
	gaussValues fittedValues;

	for (Int_t i = 0; i < numPeaks; i++)
	{
		fittedValues.amplitudes.push_back(fitFunc->GetParameter(i * 3));
		fittedValues.means.push_back(fitFunc->GetParameter(i * 3 + 1));
		fittedValues.sigmas.push_back(fitFunc->GetParameter(i * 3 + 2));

		fittedValues.meanErrors.push_back(fitFunc->GetParError(i * 3 + 1));
		fittedValues.sigmaErrors.push_back(fitFunc->GetParError(i * 3 + 2));

		TLatex *label = new TLatex(fittedValues.means[i], fittedValues.amplitudes[i] + 1000, Form("%.1f, %.1f", fittedValues.means[i], fittedValues.amplitudes[i]));
		

		label->SetTitle(Form("#splitline{%s}{%.1f, %.1f}", elements[i].c_str(), fittedValues.means[i], fittedValues.amplitudes[i]));

		label->SetTextSize(0.015);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	// Plot histogram
	//histogram->Draw(); 
	fitFunc->Draw("same");
	histogram->SetTitle(histogram->GetName());
	
	//fitFunc->Draw("same");

	// Return fitted values of gaussian
	return fittedValues;
}

/** Retrieve Gaussian peak means
 * @param h Histogram in which to find peaks
 * @param threshold Threshold according to which peaks should be considered
 * @param sigma Peak sigma
 * @param rangeMin Lower bound of range in which to find peaks
 * @param rangeMax Upper bound of range in which to find peaks
 * @return Vector containing means of each peak
 */
std::vector<Double_t> findPeaks(TH1F *histogram, Double_t threshold, Double_t sigma, Double_t rangeMin, Double_t rangeMax)
{
	// Initialize spectrum
	TSpectrum *spectrum = new TSpectrum();
	std::vector<Double_t> filteredPositions;

	spectrum->Background(histogram, 10, "same");

	// Retrieve number of peaks and their positions
	Int_t nFound = spectrum->Search(histogram, sigma, "", threshold);
	Double_t *positions = spectrum->GetPositionX();

	// Filter means outside of bounds
	for (Int_t i = 0; i < nFound; i++)
	{
		Double_t position = positions[i];

		if ((rangeMin < position) && (position < rangeMax))
		{
			filteredPositions.push_back(position);
		}
	}

	// Clear
	delete spectrum;

	// Return
	return filteredPositions;
}

/** Fit function
 * @param x x variable of function
 * @param par Parameters of fit (number of peaks, amplitudes, means, sigmas, background offset, background slope)
 * @return Fit function according to inputted parameters
 */
Double_t fitFunction(Double_t *x, Double_t *par)
{
	// Initialize function
	Double_t sum = 0;

	// Add gaussians
	Int_t i = 0;
	for (; i < numPeaks; i++)
	{
		sum += par[i * 3] * TMath::Gaus(*x, par[i * 3 + 1], par[i * 3 + 2]);
	}
	// Add background
	sum += par[i * 3] + par[i * 3 + 1] * (*x);

	return sum;
}