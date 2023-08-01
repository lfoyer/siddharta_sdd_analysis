Double_t fitFunction(Double_t *, Double_t *);
Double_t stepFunction(Double_t, Double_t);
Double_t tailFunction(Double_t, Double_t, Double_t, Double_t, Double_t);
void plotSingleFunctions(gaussValues, fitParams);
Int_t numPeaks = size(elements);

/**
 * Perform fit on given channel according to parameters
 * @param h Histogram to fit
 * @param parameters Parameters of fit
 * @return Parameters of fitted histogram
 */
gaussValues sumFitHistogram(TH1F *histogram, fitParams parameters)
{
	for (Double_t mean : parameters.initialValues.means)
	{
		std::cout << "initial ev: " << mean << std::endl;
	}
	std::cout << "numPeaks: " << numPeaks << std::endl;

	// Create fit function
	Int_t numParam = numPeaks * 3 + 2 + size(elemWithTail) * 2;
	TF1 *fitFunc = new TF1(Form("fitFunc"), fitFunction, parameters.rangeMin, parameters.rangeMax, numParam);

	// Set initial amp, mean, and sigma parameters for fit
	Int_t i = 0;
	for (; i < numPeaks; i++)
	{
		fitFunc->SetParameter(i * 3, parameters.initialValues.amplitudes[i]);
		fitFunc->SetParLimits(i * 3, parameters.initialValues.amplitudes[i] * 0.8, parameters.initialValues.amplitudes[i] * 1.3);
		fitFunc->SetParName(i * 3, Form("%s: g_amp ", elements[i].c_str()));

		fitFunc->SetParameter(i * 3 + 1, parameters.initialValues.means[i]);
		fitFunc->SetParLimits(i * 3 + 1, parameters.initialValues.means[i] * 0.99, parameters.initialValues.means[i] * 1.01);
		fitFunc->SetParName(i * 3 + 1, Form("%s: g_mea ", elements[i].c_str()));

		fitFunc->SetParameter(i * 3 + 2, parameters.initialValues.sigmas[i]);
		fitFunc->SetParLimits(i * 3 + 2, parameters.initialValues.sigmas[i] * 0.8, parameters.initialValues.sigmas[i] * 1.2);
		fitFunc->SetParName(i * 3 + 2, Form("%s: g_sig ", elements[i].c_str()));
	}

	std::vector<Double_t> tailAmp = {tail_amp["Ti_ka"], tail_amp["Fe_ka"], tail_amp["Cu_ka"], tail_amp["Sr_ka"], tail_amp["Sr_kb"]};
	std::vector<Double_t> tailBeta = {tail_beta["Ti_ka"], tail_beta["Fe_ka"], tail_beta["Cu_ka"], tail_beta["Sr_ka"], tail_beta["Sr_kb"]};

	for (Int_t numTail = 0; numTail < size(elemWithTail); numTail++)
	{
		// tail amp - now a factor to be multiplied by gauss amp
		fitFunc->SetParameter(numPeaks * 3 + numTail * 2, tailAmp[numTail]);
		fitFunc->SetParLimits(numPeaks * 3 + numTail * 2, 0, 100);
		fitFunc->SetParName(numPeaks * 3 + numTail * 2, "t_amp ");
		// beta
		fitFunc->SetParameter(numPeaks * 3 + numTail * 2 + 1, tailBeta[numTail]);
		fitFunc->SetParLimits(numPeaks * 3 + numTail * 2 + 1, 0, 5);
		fitFunc->SetParName(numPeaks * 3 + numTail * 2 + 1, "t_bet ");
	}

	// Background parameters
	fitFunc->SetParameter(i * 3 + size(elemWithTail) * 2, parameters.backgroundOffset);
	// fitFunc->SetParLimits(i * 5, parameters.backgroundOffset * 0.8, parameters.backgroundOffset * 1.2);
	fitFunc->SetParName(i * 3 + size(elemWithTail) * 2, "offset  ");
	fitFunc->SetParameter(i * 3 + size(elemWithTail) * 2 + 1, parameters.backgroundSlope);
	fitFunc->SetParName(i * 3 + size(elemWithTail) * 2 + 1, "slope  ");

	// Perform fit
	histogram->Fit(fitFunc, "R");

	// Make TRatioPlot for residuals + fetch upper pad for labelling
	auto rp1 = new TRatioPlot(histogram);
	rp1->Draw("diff");
	rp1->GetLowerRefYaxis()->SetRangeUser(-500., 500.);
	TPad *p = rp1->GetUpperPad();
	if (p)
		p->cd();

	// Retrieve fitted values and add labels
	gaussValues fittedValues;

	for (Int_t i = 0; i < numPeaks; i++)
	{
		fittedValues.amplitudes.push_back(fitFunc->GetParameter(i * 3));
		fittedValues.means.push_back(fitFunc->GetParameter(i * 3 + 1));
		fittedValues.sigmas.push_back(fitFunc->GetParameter(i * 3 + 2));

		fittedValues.meanErrors.push_back(fitFunc->GetParError(i * 3 + 1));
		fittedValues.sigmaErrors.push_back(fitFunc->GetParError(i * 3 + 2));

		TLatex *label = new TLatex(fittedValues.means[i], fittedValues.amplitudes[i] + 5000 + (fittedValues.means[i] * fitFunc->GetParameter(numParam - 1) + fitFunc->GetParameter(numParam - 2)), Form("%.1f, %.1f", fittedValues.means[i], fittedValues.amplitudes[i]));

		label->SetTitle(Form("#splitline{%s}{%.1f, %.1f}", elements[i].c_str(), fittedValues.means[i], fittedValues.amplitudes[i]));

		label->SetTextSize(0.015);
		label->SetTextColor(kRed);
		label->SetTextAlign(22);
		label->Draw();
	}

	for (Int_t i = 0; i < size(elemWithTail); i++)
	{
		fittedValues.tailAmplitudes.push_back(fitFunc->GetParameter(numPeaks * 3 + i * 2));
		fittedValues.tailBetas.push_back(fitFunc->GetParameter(numPeaks * 3 + i * 2 + 1));
	}

	plotSingleFunctions(fittedValues, parameters);

	// Return fitted values of gaussian
	return fittedValues;
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
	Int_t numTail = 0;

	for (Int_t i = 0; i < numPeaks; i++)
	{
		sum += par[i * 3] * TMath::Gaus(*x, par[i * 3 + 1], par[i * 3 + 2]);

		if (elemWithTail.count(elements[i]))
		{
			sum += tailFunction(*x, par[i * 3] * par[numPeaks * 3 + numTail * 2], par[numPeaks * 3 + numTail * 2 + 1], par[i * 3 + 1], par[i * 3 + 2]);
			numTail++;
		}
	}

	// Add background linfunc
	sum += (par[numPeaks * 3 + size(elemWithTail) * 2] + par[numPeaks * 3 + size(elemWithTail) * 2 + 1] * (*x));

	return sum;
}

Double_t tailFunction(Double_t x, Double_t amplitude, Double_t beta, Double_t mean, Double_t sigma)
{
	return (amplitude / (2 * beta * sigma)) * TMath::Exp(((x - mean) / (beta * sigma)) + (1 / (2 * beta * beta))) * TMath::Erfc((x - mean) / (TMath::Sqrt(2) * sigma) + (1 / TMath::Sqrt(2) * beta));
}

Double_t stepFunction(Double_t x, Double_t threshold)
{
	return (x > threshold) ? 1 : 0;
}

Double_t tailFunction(Double_t *x, Double_t *par)
{
	return tailFunction(*x, par[0], par[1], par[2], par[3]);
}

void plotSingleFunctions(gaussValues fittedValues, fitParams parameters)
{
	Int_t numTail = 0;

	for (Int_t i = 0; i < numPeaks; i++)
	{
		TF1 *gauss = new TF1("gauss", "gaus", parameters.rangeMin, parameters.rangeMax);
		gauss->SetParameters(fittedValues.amplitudes[i], fittedValues.means[i], fittedValues.sigmas[i]);
		gauss->SetLineColor(kViolet);
		gauss->Draw("same");

		if (elemWithTail.count(elements[i]))
		{
			TF1 *tail = new TF1("tail", tailFunction, parameters.rangeMin, parameters.rangeMax, 4);
			tail->SetParameters(fittedValues.amplitudes[i] * fittedValues.tailAmplitudes[numTail], fittedValues.tailBetas[numTail], fittedValues.means[i], fittedValues.sigmas[i]);
			tail->SetLineColor(kBlue);
			tail->Draw("same");
			numTail++;
		}
	}
}