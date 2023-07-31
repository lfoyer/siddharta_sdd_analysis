Int_t getId(TH1F* histogram)
{
    std::string str = histogram->GetName();
    size_t last_index = str.find_last_not_of("0123456789");
    std::string result = str.substr(last_index + 1);
    
    return stoi(result);
}