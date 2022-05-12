/*
 X-position: -10mm to 21mm
 Y-position: 24mm
 
 FEB 1 is at x = 3000mm
 FEB 2 is at x = 0mm
 
 Channels with 2x2 SiPM:
 Channels 0,1 correspond to counters with bare ends
 Channels 8,9 correspond to counters with reflective ends ("Craig's speical paint")
 Channels 12,15
 
 Channels with 3x3 SiPM:
 Channels 4,5
*/

# include <string>

using namespace std;

void make_pe_vs_x_plot(int feb, int sipm);
void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2);

TCanvas* canvas_1;
TCanvas* canvas_2;
char* title;
TProfile* profile_1;
TProfile* profile_2;
TFile* in_File; // file-scanning pointers
TTree* run;
double pos_x, pos_y; // desired variables to be extracted from files
int pe[2][16];
TLegend* legend_1; // legends for plots
TLegend* legend_2;
TF1* f_1; // fitting the ratio plot
TFitResultPtr result_1;

void End_Study_3(){

    make_pe_vs_x_plot(1,0);
    make_pe_vs_x_plot(1,1);
    make_pe_vs_x_plot(1,8);
    make_pe_vs_x_plot(1,9);

    make_pe_vs_x_plot(2,0);
    make_pe_vs_x_plot(2,1);
    make_pe_vs_x_plot(2,8);
    make_pe_vs_x_plot(2,9);

    make_pe_vs_x_plot_overlay(1,0,1,8);
    make_pe_vs_x_plot_overlay(1,1,1,9);
    make_pe_vs_x_plot_overlay(2,0,2,8);
    make_pe_vs_x_plot_overlay(2,1,2,9);

    make_pe_vs_x_plot_overlay_shifted(1,0,1,8);
    make_pe_vs_x_plot_overlay_shifted(1,1,1,9);
    make_pe_vs_x_plot_overlay_shifted(2,0,2,8);
    make_pe_vs_x_plot_overlay_shifted(2,1,2,9);
 
}

void make_pe_vs_x_plot(int feb, int sipm){
    canvas_1 = new TCanvas();
    //gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);

    title = Form("FEB %d Channel %d",feb,sipm);
    profile_1 = new TProfile("profile",title,1000,1450,1550);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->GetXaxis()->SetNdivisions(5,5,0);
    
    for(int i=669; i<=669; i++){
        // gets the tree at for run i
        in_File = new TFile(Form("Run_0%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        // "branches" these variables in the tree
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        // fills the profile
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35)
                profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb-1][sipm],1);
        }
        in_File->Close();
    }
    
    profile_1->Draw();
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/One_Channel/sipm_[%d][%d]_part_2.png",feb,sipm));
}

void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1450,1550);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1450,1550);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->GetXaxis()->SetNdivisions(5,5,0);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    for(int i=669; i<=669; i++){
        
        in_File = new TFile(Form("Run_0%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35){
                profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1-1][sipm_1],1);
                profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2-1][sipm_2],1);
            }
        }
        in_File->Close();
    }
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay/sipms_[%d][%d]_and_[%d][%d]_part_2.png",feb_1,sipm_1,feb_2,sipm_2));
}

void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,80,1450,1530);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,80,1450,1530);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetYaxis()->SetTitle("");
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->GetXaxis()->SetNdivisions(5,5,0);
    profile_1->GetYaxis()->SetTicks("-");
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    for(int i=669; i<=669; i++){
        
        in_File = new TFile(Form("Run_0%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35){
                profile_1->Fill((Double_t)pos_x-1.3-7.4,(Double_t)pe[feb_1-1][sipm_1],1);
                profile_2->Fill((Double_t)pos_x-7.4,(Double_t)pe[feb_2-1][sipm_2],1);
            }
        }
        in_File->Close();
    }
    legend_1 = new TLegend(0.885, 0.1525, 0.425, 0.325, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("[%d][%d] Mean = %6.5lf",feb_1,sipm_1,(double)profile_1->GetMean(2)));
    legend_1->AddEntry(profile_2, Form("[%d][%d] Mean = %6.5lf",feb_2,sipm_2,(double)profile_2->GetMean(2)));
    legend_1->AddEntry("difference",Form("[%d][%d]-[%d][%d] = %6.5lf",feb_1,sipm_1,feb_2,sipm_2,(double)(profile_1->GetMean(2)-profile_2->GetMean(2))));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted/sipms_[%d][%d]_and_[%d][%d]_part_2.png",feb_1,sipm_1,feb_2,sipm_2));
}
