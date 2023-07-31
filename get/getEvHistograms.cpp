std::vector<TH1F*> getEvHistograms(TFile* file) {
    std::cout << "Reading from file: " << file->GetName() << std::endl;
    std::vector<TH1F*> histograms;

    // Get the subdirectory named "evGraphs"
    TDirectory* subdir = dynamic_cast<TDirectory*>(file->Get("evGraph"));
    if (!subdir) {
        std::cerr << "Subdirectory 'evGraphs' not found in the ROOT file." << std::endl;
        return histograms;
    }

    // Iterate through all keys in the subdirectory
    TIter nextKey(subdir->GetListOfKeys());
    TKey* key = nullptr;
    while ((key = static_cast<TKey*>(nextKey()))) {
        // Get name and class of the key
        const char* keyName = key->GetName();
        const char* className = key->GetClassName();

        // Check if the key represents a histogram (TH1F)
        if (strcmp(className, "TH1F") == 0) {
            TH1F* h = dynamic_cast<TH1F*>(key->ReadObj());
            if (h) {
                std::cout << "Loading " << keyName << std::endl;

                // Filter out faulty histograms (threshold set to 1000)
                if (h->GetMaximum() > 1000) {
                    histograms.push_back(h);
                } else {
                    std::cout << "!!! " << keyName << " skipped: max too low at " << h->GetMaximum() << std::endl;
                    delete h;  // Delete skipped histogram
                }
            }
        }
    }

    // Return as vector of TH1F* histograms
    return histograms;
}