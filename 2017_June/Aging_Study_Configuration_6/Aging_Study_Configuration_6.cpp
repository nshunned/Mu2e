//  Aging_Study_Configuration_6.cpp
//  Created by Ningshun Chen on 12/19/17

/*
 Aging study comparing PE yield between light yield in 2016 and 2017 for the same counters.
 
 Aged counters are in Configuration 6:
 
 FEB 1 is at x = 0mm
 FEB 2 is at x = 3000mm
 
 1) filled with silicone
 [0][0] --- [1][0] 25mm (bottom)
 [0][1] --- [1][1]
 [0][2] --- [1][2] 75mm (top)
 [0][3] --- [1][3]
 
 2) dicounters from 2016
 [0][4] --- [1][4]
 [0][5] --- [1][5]
 [0][6] --- [1][6]
 [0][7] --- [1][7]
 
 3) dicounters from 2016
 [0][8] --- [1][8]
 [0][9] --- [1][9]
 [0][10] --- [1][10]
 [0][11] --- [1][11]
 
 4) dicounters from 2016
 [0][12] --- [1][12]
 [0][13] --- [1][13]
 [0][14] --- [1][14]
 [0][15] --- [1][15]
 */

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const double FIT_X_MIN = 50, FIT_X_MAX = 500;
vector<double> mpv_vec_2017; // special counter MPVs
vector<double> mpv_vec_2016; // default counter MPVs
vector<double> mpv_vec_ratio; // MPV ratios: special/default

void set_canvas_style();
void set_axes(TProfile* profile, const char* x_title, const char* y_title);
TTree* get_tree_from_run(int run_num);
void make_pe_distribution_plot(int run_num_2017, int run_num_2016, int sipm_2017, int simp_2016);
TF1* get_fit(TH1F* pe_distribution, int color);
void print_result();

void Aging_Study_Configuration_6(){
    
    // counter label 110
    
    // t-t & b-b
    // 2017-run1339 & 2016-run831: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1339,831,4,6);
    // 2017-run1338 & 2016-run832: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1338,832,6,4);
    
    // 2017-run1334 & 2016-run827: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1334,827,4,6);
    // 2017-run1335 & 2016-run828: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1335,828,6,4);
    
    // 2017-run1329 & 2016-run823: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1329,823,4,6);
    // 2017-run1330 & 2016-run824: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1330,824,6,4);
     
    /*
    // t-b & b-t
    // 2017-run1339 & 2016-run832: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1339,832,4,4);
    // 2017-run1338 & 2016-run831: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1338,831,6,6);
    
    // 2017-run1334 & 2016-run828: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1334,828,4,4);
    // 2017-run1335 & 2016-run827: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1335,827,6,6);
    
    // 2017-run1329 & 2016-run824: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1329,824,4,4);
    // 2017-run1330 & 2016-run823: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1330,823,6,6);
     */
     
    
    // counter label 109
    /*
    // t-t & b-b
    // 2017-run1339 & 2016-run807: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1339,807,8,6);
    // 2017-run1338 & 2016-run806: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1338,806,10,4);
    
    // 2017-run1334 & 2016-run812: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1334,812,8,6);
    // 2017-run1335 & 2016-run810: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1335,810,10,4);
    
    // 2017-run1329 & 2016-run816: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1329,816,8,6);
    // 2017-run1330 & 2016-run815: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1330,815,10,4);
     */
    /*
    // t-b & b-t
    // 2017-run1338 & 2016-run806: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1338,806,10,4);
    // 2017-run1339 & 2016-run807: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1339,807,8,6);
    
    // 2017-run1335 & 2016-run810: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1335,810,10,4);
    // 2017-run1334 & 2016-run812: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1334,812,8,6);
    
    // 2017-run1330 & 2016-run815: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1330,815,10,4);
    // 2017-run1329 & 2016-run816: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1329,816,8,6);
     */
     
    print_result();
}

void set_canvas_style(){
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
}

void set_axes(TH1F* distribution, const char* x_title, const char* y_title){
    distribution->GetXaxis()->SetTitle(x_title);
    distribution->GetYaxis()->SetTitle(y_title);
    distribution->GetXaxis()->SetTitleSize(0.05);
    distribution->GetYaxis()->SetTitleSize(0.05);
    distribution->GetXaxis()->SetTitleOffset(1.1);
    distribution->GetYaxis()->SetTitleOffset(0.5);
    distribution->GetXaxis()->SetLabelSize(0.06);
    distribution->GetYaxis()->SetLabelSize(0);
    distribution->GetYaxis()->SetRangeUser(0,700);
}

TTree* get_tree_from_run(int run_num){
    gSystem->cd("../");
    TFile* file;
    if(run_num < 1002) {
        gSystem->cd("../2016_June");
        file = new TFile(Form("Run_0%dmerged.root", run_num),"READ");
        gSystem->cd("../2017_June");
    }
    else {
        file = new TFile(Form("Run_%dmerged.root", run_num),"READ");
    }
    gSystem->cd("Aging_Study_Configuration_6");
    
    if(file == NULL)
        return NULL;
    if(file->GetListOfKeys()->Contains(Form("run%d", run_num))){
        TTree* run = (TTree*)file->Get(Form("run%d", run_num));
        if(run->GetEntries() < 3000)
            return NULL;
        return run;
    }
    return NULL;
}

void make_pe_distribution_plot(int run_num_2017, int run_num_2016, int sipm_2017, int sipm_2016){
    TTree* run_2017 = get_tree_from_run(run_num_2017);
    TTree* run_2016 = get_tree_from_run(run_num_2016);
    
    // draws & gets the histograms from data
    TCanvas* canvas = new TCanvas();
    set_canvas_style();
    char* bounds_2017 = Form("PEs_[0][%d] > 30 && valid_[0][%d] && PEs_[1][%d] > 30 && valid_[1][%d] && PEs_[0][%d] > 30 && valid_[0][%d] && PEs_[1][%d] > 30 && valid_[1][%d]",sipm_2017,sipm_2017,sipm_2017,sipm_2017,sipm_2017+1,sipm_2017+1,sipm_2017+1,sipm_2017+1);
    char* bounds_2016 = Form("PEs_[0][%d] > 30 && valid_[0][%d] && PEs_[1][%d] > 30 && valid_[1][%d] && PEs_[0][%d] > 30 && valid_[0][%d] && PEs_[1][%d] > 30 && valid_[1][%d]",sipm_2016,sipm_2016,sipm_2016,sipm_2016,sipm_2016+1,sipm_2016+1,sipm_2016+1,sipm_2016+1);
    run_2017->Draw(Form("PEs_[0][%i]+PEs_[1][%i]+PEs_[0][%i]+PEs_[1][%i] >> pe_distr_2017(800,0,800)",sipm_2017,sipm_2017,sipm_2017+1,sipm_2017+1),bounds_2017);
    run_2016->Draw(Form("PEs_[0][%i]+PEs_[1][%i]+PEs_[0][%i]+PEs_[1][%i] >> pe_distr_2016(800,0,800)",sipm_2016,sipm_2016,sipm_2016+1,sipm_2016+1),bounds_2016);
    
    TH1F* pe_distr_2017 = (TH1F*) gDirectory->FindObjectAny("pe_distr_2017");
    TH1F* pe_distr_2016 = (TH1F*) gDirectory->FindObjectAny("pe_distr_2016");

    // sets histogram syles
    pe_distr_2017->SetTitle("");
    set_axes(pe_distr_2017, "Photoelectrons (PE)", "Unit normalized");
    
    pe_distr_2017->SetFillStyle(3354);
    pe_distr_2017->SetFillColor(1);
    pe_distr_2017->SetLineColor(1); // 1 - black
    pe_distr_2017->SetLineWidth(1);
    pe_distr_2017->SetLineStyle(1);
    
    pe_distr_2016->SetFillStyle(3345);
    pe_distr_2016->SetFillColor(1);
    pe_distr_2016->SetLineColor(1);
    pe_distr_2016->SetLineWidth(1);
    pe_distr_2016->SetLineStyle(1);
    
    // gets & fits fitted histogram distributions
    TF1* pe_distr_2017_fit = get_fit(pe_distr_2017,2); // 2 - red
    TF1* pe_distr_2016_fit = get_fit(pe_distr_2016,4); // 4 - blue
    pe_distr_2017->Fit(pe_distr_2017_fit, "0QR+");
    pe_distr_2016->Fit(pe_distr_2016_fit, "0QR+");
    
    // get MPVs
    double mpv_2017 = pe_distr_2017_fit->GetMaximumX(FIT_X_MIN,FIT_X_MAX);
    double mpv_2016 = pe_distr_2016_fit->GetMaximumX(FIT_X_MIN,FIT_X_MAX);
    mpv_vec_2017.push_back(mpv_2017);
    mpv_vec_2016.push_back(mpv_2016);

    // sets a legend
    TLegend* legend = new TLegend(0.875, 0.875, 0.6, 0.675, "Most probable values:");
    legend->AddEntry(pe_distr_2016, Form("2016 run: %3.1f PE", mpv_2016), "F");
    legend->AddEntry(pe_distr_2017, Form("2017 run: %3.1f PE", mpv_2017), "F");
    legend->SetTextSize(.035);
    legend->SetBorderSize(0);
    
    
    // draws the final histograms
    pe_distr_2017->Draw("");
    pe_distr_2016->Draw("SAME");
    legend->Draw();
    
    // saves the histogram as a .png file
    char* plot_file_name;
    if(sipm_2017%4 < 2)
        plot_file_name = Form("PE_Distribution_Plots/run%i_and_run%i_bottom_counter.png",run_num_2017,run_num_2016);
    else
        plot_file_name = Form("PE_Distribution_Plots/run%i_and_run%i_top_counter.png",run_num_2017,run_num_2016);

    canvas->Print(plot_file_name);

    //canvas->Close();
}

TF1* get_fit(TH1F* pe_distribution, int color){
    // fits pe_distribution with half half distributions: Gaussian & Landau
    pe_distribution->GetXaxis()->SetRangeUser(FIT_X_MIN, FIT_X_MAX);
    int maxbin = pe_distribution->GetMaximumBin();
    int midPE = pe_distribution->GetBinCenter(maxbin);
    TF1* gau  = new TF1("gau", "gaus", FIT_X_MIN, midPE);
    TF1* lan  = new TF1("lan", "landau", midPE, FIT_X_MAX);
    pe_distribution->Fit(gau,"0QR");     // 0 : do not plot the result of the fit
    pe_distribution->Fit(lan,"0QR+");    // Q : quiet mode (min printing)
    Double_t par[6];                     // R : use range specified in the function range
    gau->GetParameters(&par[0]);         // + : add this fitted function to the list of fitted funtions
    lan->GetParameters(&par[3]);         //     (by default, any previous fitted function is deleted)
    
    // fits pe_distribution w a single distribution: Gaussian+Landau
    TF1 *lgau = new TF1("lgau","gaus(0)+landau(3)", FIT_X_MIN, FIT_X_MAX);
    lgau->SetParameters(par);
    lgau->SetLineColor(color);
    lgau->SetLineWidth(2);
    pe_distribution->GetXaxis()->SetRangeUser(0, 800);
    
    return lgau;
}

void print_result() {
    ofstream myFile;
    myFile.open("ratios.txt");
    
    cout << "MPV 2017 / MPV 2016" << endl;
    myFile << "MPV 2017 / MPV 2016" << endl;
    
    double sum = 0;
    for(int i = 0; i < mpv_vec_2017.size(); i++) {
        mpv_vec_ratio.push_back(mpv_vec_2017.at(i)/mpv_vec_2016.at(i));
        sum += mpv_vec_ratio.at(i);
        
        cout << setprecision(7) << mpv_vec_ratio.at(i) << "      ";
        myFile << setprecision(7) << mpv_vec_ratio.at(i) << "      ";
        
        if(i%2 == 1) cout << endl;
    }
    cout << sum/mpv_vec_ratio.size() << endl;
    myFile << sum/mpv_vec_ratio.size() << endl;
}
