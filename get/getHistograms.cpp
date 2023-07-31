/**
 * Read ROOT file contents
 * @param file Pointer to ROOT file
 * @return Histograms extracted from ROOT file
 */
std::vector<TH1F *> getHistograms(TFile *file)
{
	std::cout << "Reading from file: " << file->GetName() << std::endl;
	std::vector<TH1F *> histograms;

	// Iterate through all channels
	for (auto &&keyAsObj : *file->GetListOfKeys())
	{
		// Get name
		auto *key = static_cast<TKey *>(keyAsObj);
		TH1F *h = file->Get<TH1F>(key->GetName());

		std::cout << "loading " << key->GetName() << endl;

		// Load histogram to vector, filter out faulty histograms (threshold set to 1000)
		if (h->GetMaximum() > 1000)
		{
			histograms.push_back(h);
		}
		else
		{
			cout << "!!! " << h->GetName() << " skipped: max too low at " << h->GetMaximum() << endl;
		}
	}

	// Return as vector of TH1F* histograms
	return histograms;
}