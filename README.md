# siddharta_sdd_analysis
Code used for SDD calibration and analysis of energy response

Code is far from optimal
- Run calibrate.cpp to calibrate individual histograms (ADC to eV) and define various single SDD plots; results are stored in "output/output.root" (to be used for sum.cpp), and calibration parameters in "output/calibration_settings.json"
- Run sum.cpp to sum calibrated histograms (imports "output/graphs.root"), and perform fits and various analysis plots (results stored in "output/sum.root"). Expect a couple minutes of processing due to amount of fit parameters
- Peak initial parameters are imported from "data/values"


// Data from 20210317_1139_0317_2011_Lab_xray_30kv_6ua_140HV_board_6_9_661_all_hist.root
// Laurent Foyer, Jul-Aug 2023
