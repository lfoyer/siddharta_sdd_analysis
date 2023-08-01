Double_t conversionFitFunction(Double_t *, Double_t *);

/**
 * Establish linear equation of relationship between eV / ADC energy values
 * @param n Number of points
 * @param ADCEnergy Energy values expressed in ADC arbitrary units
 * @param eVEnergy Theoretical energy values expressed in eV
 * @param rangeMin Lower bound of range
 * @param rangeMax Upper bound of range
 * @return Parameters of fitted linear function
 */
calibrationSettings conversionFunction(Int_t n, Double_t *eVEnergy, Double_t *ADCEnergy, Double_t rangeMin, Double_t rangeMax)
{
	// Plot points on TGraph
	TGraph *conversionGraph = new TGraph(n);
	conversionGraph->SetTitle("ADC-eV conversionGraph");

	for (int i = 0; i < n; i++)
	{
		conversionGraph->SetPoint(i, ADCEnergy[i], eVEnergy[i]);
	}

	// Define fit function
	TF1 *conversionFitFunc = new TF1("conversionFitFunc", conversionFitFunction, rangeMin, rangeMax, 2);
	conversionFitFunc->SetParameter(0, 3);
	conversionFitFunc->SetParameter(1, -1700);

	// Plot real function vs fit
	conversionGraph->Draw("AP");
	conversionGraph->SetMarkerColor(kBlue);
	conversionGraph->SetMarkerStyle(kFullCircle);

	conversionGraph->Fit(conversionFitFunc, "R");
	conversionGraph->Draw("same");

	// Retrieve function parameters
	calibrationSettings fittedFunction;
	fittedFunction.slope = conversionFitFunc->GetParameter(0);
	fittedFunction.intercept = conversionFitFunc->GetParameter(1);
	fittedFunction.graph = conversionGraph;
	std::cout << fittedFunction.slope << "*x + " << fittedFunction.intercept << std::endl;

	// Labels for plot
	TLatex *label = new TLatex(4500, 5000, Form("%f*x + %f", fittedFunction.slope, fittedFunction.intercept));
	label->SetTextSize(0.03);
	label->SetTextColor(kRed);
	label->SetTextAlign(22);
	label->Draw();


	for (Int_t i = 0; i < n; i++)
	{
		TLatex *label = new TLatex(ADCEnergy[i], eVEnergy[i] + 1000, Form("#splitline{%s}{%.1f ADC, %.1f eV}", mainElements[i].c_str(), ADCEnergy[i], ADCEnergy[i]*fittedFunction.slope+fittedFunction.intercept));
		label->SetTextSize(0.015);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	// Return fitted function
	return fittedFunction;
}

/**
 * Conversion fit function
 * @param x x variable of function
 * @param par Parameters of fit (linear function parameters)
 * @return Fit function according to inputted parameters
 */
Double_t conversionFitFunction(Double_t *x, Double_t *par)
{
	return par[0] * (*x) + par[1];
}