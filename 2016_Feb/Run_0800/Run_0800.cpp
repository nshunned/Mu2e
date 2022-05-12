# include <iomanip>
# include <fstream>
# include <sstream>
# include <ostream>
# include <string>
# include <cassert>
# include <vector>
# include <map>
# include <algorithm>
# include "TFitResult.h"
# include "TCanvas.h"
# include "TROOT.h"
# include "TGraphErrors.h"
# include "TH1.h"
# include "TH1F.h"
# include "TH2F.h"
# include "TF1.h"
# include "TLegend.h"
# include "TLatex.h"
# include "TStyle.h"
# include "TApplication.h"
# include "TMultiGraph.h"
# include "TMath.h"
# include "TTree.h"
# include "TFile.h"
# include "TLine.h"
# include "TSpectrum.h"
# include "THStack.h"
# include "TNtuple.h"
# include <unistd.h>
# include <sys/types.h>
# include <pwd.h>

void make_position_plot(TTree* run);
void make_sipm_pe_difference_plot(TTree* run, int i_feb, int i_sipm_1, int i_sipm_2);
void make_pe_distribution_plot(TTree* run, int i_feb, int i_sipm_1, int i_sipm_2);

void Run_0800(){
    TFile* in_File = new TFile("Run_0800merged.root");
    
    TTree* run0800;
    in_File->GetObject("run800",run0800);
    
    make_position_plot(run0800);
    make_sipm_pe_difference_plot(run0800,0,6,7);
    for(int feb_i = 0; feb_i < 2; feb_i++){
        make_pe_distribution_plot(run0800,feb_i,2,3);
        make_pe_distribution_plot(run0800,feb_i,6,7);
        make_pe_distribution_plot(run0800,feb_i,10,11);
        make_pe_distribution_plot(run0800,feb_i,14,15);
    }
}

void make_position_plot(TTree* run) {
    // draws the histogram from data
    auto canvas = new TCanvas();
    run->Draw("position_y_:position_x_ >> positions(200,235,355,200,-40,85)");
    
    // gets the histogram
    auto positions = (TH2F*) gDirectory->FindObjectAny("positions");
    
    // makes the histogram look nice
    positions->SetTitle("Positions of Proton Collisions with Dicounter");
    positions->GetXaxis()->SetTitle("x-position (mm)");
    positions->GetYaxis()->SetTitle("y-position (mm)");
    
    // draws the final histogram
    positions->Draw();
   
    // saves the histogram as a .png file
    string out_name("My_Plots/run0800_position_plot.png");
    canvas->Print(out_name.c_str(), "png");
}

void make_sipm_pe_difference_plot(TTree* run, int i_feb, int i_sipm_1, int i_sipm_2){
    // draws the histogram from data
    auto canvas = new TCanvas();
    string feb = to_string(i_feb);
    string sipm_1 = to_string(i_sipm_1);
    string sipm_2 = to_string(i_sipm_2);
    string arg1 = "PEs_["+feb+"]["+sipm_2+"]-PEs_["+feb+"]["+sipm_1+"] >> pe_diff(100,-50,50)";
    string arg2 = "PEs_["+feb+"]["+sipm_2+"] > 0 && PEs_["+feb+"]["+sipm_1+"] > 0";
    run->Draw(arg1.c_str(),arg2.c_str());
    
    // gets the histogram
    auto pe_diff = (TH1F*) gDirectory->FindObjectAny("pe_diff");
    
    // normalizes histogram pe_diff
    pe_diff->Scale(1./(pe_diff->Integral()));
    
    // makes histogram look nice
    string arg3 = "Difference in PE Yields of SiPMs "+sipm_1+" and "+sipm_2+" of FEB "+feb;
    pe_diff->SetTitle(arg3.c_str());
    pe_diff->GetXaxis()->SetTitle("Difference in PE Yields (# PEs)");
    pe_diff->GetYaxis()->SetTitle("Normalized Frequency");
    pe_diff->GetYaxis()->SetTitleOffset(1.4);
    pe_diff->SetLineColor(42);
    pe_diff->SetFillColor(42);
    pe_diff->SetFillStyle(3002);
    
    // draws the final histogram
    pe_diff->Draw("HIST");
    
    // saves the histogram as a .png file
    string arg4 = "My_Plots/run0800_sipms_["+feb+"]["+sipm_1+"]_and_["+feb+"]["+sipm_2+"]_pe_difference.png";
    canvas->Print(arg4.c_str());
}

void make_pe_distribution_plot(TTree* run, int i_feb, int i_sipm_1, int i_sipm_2){
    // draws the histograms from data
    auto canvas = new TCanvas();
    gStyle->SetOptStat(0);
    string feb = to_string(i_feb);
    string sipm_1 = to_string(i_sipm_1);
    string sipm_2 = to_string(i_sipm_2);
    string arg1 = "PEs_["+feb+"]["+sipm_1+"] >> pe_distr_1(250,0,250)";
    string arg2 = "PEs_["+feb+"]["+sipm_2+"] >> pe_distr_2(250,0,250)";
    string arg3 = "PEs_["+feb+"]["+sipm_1+"]+PEs_["+feb+"]["+sipm_2+"] >> pe_distr_3(250,0,250)";
    run->Draw(arg1.c_str());
    run->Draw(arg2.c_str());
    run->Draw(arg3.c_str());
    
    // gets the histograms
    auto pe_distr_1 = (TH1F*) gDirectory->FindObjectAny("pe_distr_1");
    auto pe_distr_2 = (TH1F*) gDirectory->FindObjectAny("pe_distr_2");
    auto pe_distr_3 = (TH1F*) gDirectory->FindObjectAny("pe_distr_3");
    
    // fits pe_distr_3 w half half distributions: Gaussian & Landau
    pe_distr_3->GetXaxis()->SetRangeUser(5, 125);
    int maxbin = pe_distr_3->GetMaximumBin();
    int midPE = pe_distr_3->GetBinCenter(maxbin);
    TF1 *gau  = new TF1("gau", "gaus", 10, midPE);
    TF1 *lan  = new TF1("lan", "landau", midPE, 125);
    pe_distr_3->Fit(gau,"0QR");     // 0 : do not plot the result of the fit
    pe_distr_3->Fit(lan,"0QR+");    // Q : quiet mode (min printing)
    Double_t par[6];                // R : use range specified in the function range
    gau->GetParameters(&par[0]);    // + : add this fitted function to the list of fitted funtions
    lan->GetParameters(&par[3]);    //     (by default, any previous fitted function is deleted)
    // fits pe_distr_3 w a single distribution: Gaussian+Landau
    TF1 *lgau = new TF1("lgau","gaus(0)+landau(3)", 0, 125);
    lgau->SetParameters(par);
    lgau->SetLineColor(kGreen);
    lgau->SetLineWidth(4);
    pe_distr_3->GetXaxis()->SetRangeUser(0, 250);
    pe_distr_3->Fit(lgau, "QR+");
    
    // sets colors of the histograms for distinction
    pe_distr_1->SetLineColor(kRed);
    pe_distr_1->SetLineWidth(1);
    pe_distr_1->SetLineStyle(1);
    pe_distr_2->SetLineColor(kBlue);
    pe_distr_2->SetLineWidth(1);
    pe_distr_2->SetLineStyle(1);
    pe_distr_3->SetLineColor(kBlack);
    pe_distr_3->SetLineWidth(1);
    pe_distr_3->SetLineStyle(1);
    
    // sets titles
    pe_distr_1->SetTitle("Run 800 PE Distribution");
    pe_distr_1->GetXaxis()->SetTitle("# PE");
    pe_distr_1->GetYaxis()->SetTitle("Counts");
    pe_distr_1->GetYaxis()->SetTitleOffset(1.4);
    
    // sets a legend
    string legend_title = "Run 800 merged "+to_string((long)run->GetEntries())+" events";
    auto legend = new TLegend(0.9, 0.9, 0.55, 0.65, legend_title.c_str());
    string arg5 = "FEB "+feb+" channel "+sipm_1;
    string arg6 = "FEB "+feb+" channel "+sipm_2;
    legend->AddEntry(pe_distr_1, arg5.c_str());
    legend->AddEntry(pe_distr_2, arg6.c_str());
    legend->AddEntry(pe_distr_3, "Total");
    legend->AddEntry(lgau, "Fit");
    
    // draws the final histograms
    pe_distr_1->Draw();
    pe_distr_2->Draw("SAME");
    pe_distr_3->Draw("SAME");
    legend->Draw();
    
    // saves the histogram as a .png file
    string arg7 = "My_Plots/run0800_sipms_["+feb+"]["+sipm_1+"]_and_["+feb+"]["+sipm_2+"]_pe_distribution.png";
    canvas->Print(arg7.c_str());
}
