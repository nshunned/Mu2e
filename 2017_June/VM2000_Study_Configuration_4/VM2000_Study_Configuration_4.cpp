//  Reflector_Study_Configuration_1.cpp
//  Created by Ningshun Chen on 12/22/17.

/*
 PE yield study comparing VM2000 & regular counters.
 
 Uses runs from Configuration 4:
 
 FEB 1 is at x = 0mm
 FEB 2 is at x = 3000mm
 
 1) filled with silicone
 [0][0] --- [1][0] 25mm (bottom)
 [0][1] --- [1][1]
 [0][2] --- [1][2] 75mm (top)
 [0][3] --- [1][3]
 
 2) optical grease at both sides, black tape at side A
 [0][4] --- [1][4]
 [0][5] --- [1][5]
 [0][6] --- [1][6]
 [0][7] --- [1][7]
 
 3) expensive reflector (VM2000) at side A
 [0][8] --- [1][8]
 [0][9] --- [1][9]
 [0][10] --- [1][10]
 [0][11] --- [1][11]
 
 4) white painted ends
 [0][12] --- [1][12]
 [0][13] --- [1][13]
 [0][14] --- [1][14]
 [0][15] --- [1][15]
 */

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const double FIT_X_MIN = 50, FIT_X_MAX = 350;
vector<double> mpv_vec_spe; // special counter MPVs
vector<double> mpv_vec_def; // default counter MPVs
vector<double> mpv_vec_ratio; // MPV ratios: special/default

void set_canvas_style();
void set_axes(TProfile* profile, const char* x_title, const char* y_title);
TTree* get_tree_from_run(int run_num);
void make_pe_distribution_plot(int run_num_spe, int run_num_def, int sipm_spe, int sipm_def);
TF1* get_fit(TH1F* pe_distribution, int color);
void print_result();

void VM2000_Study_Configuration_4(){
    // counter label 275
    
    /*
    // VM2000-run1262 & default-run1040: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1262,1040,8,4);
    // VM2000-run1251 & default-run1061: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1251,1061,10,6);
    
    // VM2000-run1260 & default-run1038: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1260,1038,8,4);
    // VM2000-run1253 & default-run1063: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1253,1063,10,6);
    
    // VM2000-run1258 & default-run1035: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1258,1035,8,4);
    // VM2000-run1255 & default-run1066: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1255,1066,10,6);
    */
    
    // VM2000-run1262 & default-run1040: beam x = 1000 mm, y = 25 mm
    make_pe_distribution_plot(1262,1040,8,8);
    // VM2000-run1251 & default-run1061: beam x = 1000 mm, y = 75 mm
    make_pe_distribution_plot(1251,1061,10,10);
    
    // VM2000-run1260 & default-run1038: beam x = 1500 mm, y = 25 mm
    make_pe_distribution_plot(1260,1038,8,8);
    // VM2000-run1253 & default-run1063: beam x = 1500 mm, y = 75 mm
    make_pe_distribution_plot(1253,1063,10,10);
    
    // VM2000-run1258 & default-run1035: beam x = 2000 mm, y = 25 mm
    make_pe_distribution_plot(1258,1035,8,8);
    // VM2000-run1255 & default-run1066: beam x = 2000 mm, y = 75 mm
    make_pe_distribution_plot(1255,1066,10,10);
    
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
    distribution->GetYaxis()->SetRangeUser(0,1200);
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
    gSystem->cd("VM2000_Study_Configuration_4");
    
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

void make_pe_distribution_plot(int run_num_spe, int run_num_def, int sipm_spe, int sipm_def){
    TTree* run_spe = get_tree_from_run(run_num_spe);
    TTree* run_def = get_tree_from_run(run_num_def);
    
    // draws & gets the histograms from data
    TCanvas* canvas = new TCanvas();
    set_canvas_style();
    char* bounds_spe = Form("PEs_[1][%d] > 30 && valid_[1][%d] && PEs_[1][%d] > 30 && valid_[1][%d]",sipm_spe,sipm_spe,sipm_spe+1,sipm_spe+1);
    char* bounds_def = Form("PEs_[1][%d] > 30 && valid_[1][%d] && PEs_[1][%d] > 30 && valid_[1][%d]",sipm_def,sipm_def,sipm_def+1,sipm_def+1);
    run_spe->Draw(Form("PEs_[1][%i]+PEs_[1][%i] >> pe_distr_spe(400,0,400)",sipm_spe,sipm_spe+1),bounds_def);
    run_def->Draw(Form("PEs_[1][%i]+PEs_[1][%i] >> pe_distr_def(400,0,400)",sipm_def,sipm_def+1),bounds_spe);
    
    TH1F* pe_distr_spe = (TH1F*) gDirectory->FindObjectAny("pe_distr_spe");
    TH1F* pe_distr_def = (TH1F*) gDirectory->FindObjectAny("pe_distr_def");
    
    // sets histogram syles
    pe_distr_spe->SetTitle("");
    set_axes(pe_distr_spe, "Photoelectrons (PE)", "Unit normalized");
    
    pe_distr_spe->SetFillStyle(3354);
    pe_distr_spe->SetFillColor(1);
    pe_distr_spe->SetLineColor(1); // 1 - black
    pe_distr_spe->SetLineWidth(1);
    pe_distr_spe->SetLineStyle(1);
    
    pe_distr_def->SetFillStyle(3345);
    pe_distr_def->SetFillColor(1);
    pe_distr_def->SetLineColor(1);
    pe_distr_def->SetLineWidth(1);
    pe_distr_def->SetLineStyle(1);
    
    // gets & fits fitted histogram distributions
    TF1* pe_distr_spe_fit = get_fit(pe_distr_spe,2); // 2 - red
    TF1* pe_distr_def_fit = get_fit(pe_distr_def,4); // 4 - blue
    pe_distr_spe->Fit(pe_distr_spe_fit, "0QR+");
    pe_distr_def->Fit(pe_distr_def_fit, "0QR+");
    
    // get MPVs
    double mpv_spe = pe_distr_spe_fit->GetMaximumX(FIT_X_MIN,FIT_X_MAX);
    double mpv_def = pe_distr_def_fit->GetMaximumX(FIT_X_MIN,FIT_X_MAX);
    mpv_vec_spe.push_back(mpv_spe);
    mpv_vec_def.push_back(mpv_def);
    
    // sets a legend
    TLegend* legend = new TLegend(0.875, 0.875, 0.6, 0.675, "Most probable values:");
    legend->AddEntry(pe_distr_def, Form("Default: %3.1f PE", mpv_def), "F");
    legend->AddEntry(pe_distr_spe, Form("VM2000: %3.1f PE", mpv_spe), "F");
    legend->SetTextSize(.035);
    legend->SetBorderSize(0);
    
    // draws the final histograms
    pe_distr_spe->Draw("");
    pe_distr_def->Draw("SAME");
    legend->Draw();
    
    // saves the histogram as a .png file
    char* plot_file_name;
    if(sipm_spe%4 < 2)
        plot_file_name = Form("PE_Distribution_Plots/run%i_and_run%i_bottom_counter.png",run_num_spe,run_num_def);
    else
        plot_file_name = Form("PE_Distribution_Plots/run%i_and_run%i_top_counter.png",run_num_spe,run_num_def);
    
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
    pe_distribution->GetXaxis()->SetRangeUser(0, 400);
    
    return lgau;
}

void print_result() {
    ofstream myFile;
    myFile.open("ratios.txt");
    
    cout << "MPV VM2000 / MPV Default" << endl;
    myFile << "MPV VM2000 / MPV Default" << endl;
    
    double sum = 0;
    for(int i = 0; i < mpv_vec_spe.size(); i++) {
        mpv_vec_ratio.push_back(mpv_vec_spe.at(i)/mpv_vec_def.at(i));
        sum += mpv_vec_ratio.at(i);
        
        cout << setprecision(7) << mpv_vec_ratio.at(i) << "      ";
        myFile << setprecision(7) << mpv_vec_ratio.at(i) << "      ";
        
        if(i%2 == 1) cout << endl;
    }
    cout << sum/mpv_vec_ratio.size() << endl;
    myFile << sum/mpv_vec_ratio.size() << endl;
}
