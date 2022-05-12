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
void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2); // 1 nonreflective; 2 reflective
void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_pe_vs_x_plot_overlay_shifted_low(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_sum_scaled_plot();

TCanvas* canvas_1;
TCanvas* canvas_2;
TCanvas* canvas_3;
char* title;
TProfile* profile_1;
TProfile* profile_2;
TProfile* profile_3;
TProfile* profile_4;
TProfile* profile_5;
TProfile* profile_6;
TH1D* projection_1;
TH1D* projection_2;
TFile* in_File; // file-scanning pointers
TTree* run;
double pos_x, pos_y; // desired variables to be extracted from files
int pe[2][16];
TLegend* legend_1; // legends for plots
TLegend* legend_2;
TF1* f_1; // fitting the ratio plot
TFitResultPtr result_1;

void End_Study_2(){
    /*
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
    
    make_pe_vs_x_plot_overlay_shifted_low(1,0,1,8);
    make_pe_vs_x_plot_overlay_shifted_low(1,1,1,9);
    make_pe_vs_x_plot_overlay_shifted_low(2,0,2,8);
    make_pe_vs_x_plot_overlay_shifted_low(2,1,2,9);
    
    
    make_pe_vs_x_plot(1,8);
    make_pe_vs_x_plot_overlay(1,0,1,8);
    make_pe_vs_x_plot_overlay_shifted(1,0,1,8);
    make_pe_vs_x_plot_overlay_shifted_low(1,0,1,8);
    
    make_pe_vs_x_plot(2,0);
    make_pe_vs_x_plot_overlay(2,0,2,8);
    make_pe_vs_x_plot_overlay_shifted(2,0,2,8);
    make_pe_vs_x_plot_overlay_shifted_low(2,0,2,8);
    */
    
    make_sum_scaled_plot();
    
    //make_pe_vs_x_plot_overlay_shifted(1,0,1,8);
}

void make_pe_vs_x_plot(int feb, int sipm){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("FEB %d Channel %d",feb,sipm);
    profile_1 = new TProfile("profile",title,1400,0,140);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    
    //for(int i=706; i<=725; i++){
    for(int i=719; i<=719; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;
        if(i == 715 && feb == 1 && sipm == 8) continue;
        if(i == 718 && feb == 2 && sipm == 0) continue;
        
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
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/One_Channel/sipm_[%d][%d]_part_1.png",feb,sipm),"png");
    
    profile_1->GetXaxis()->SetRangeUser(0,30);
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/One_Channel_Low/sipm_[%d][%d].png",feb,sipm),"png");
}

void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,140,0,140);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,140,0,140);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    for(int i=706; i<=725; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;
        if(i == 715 && feb_2 == 1 && sipm_2 == 8) continue;
        if(i == 718 && feb_1 == 2 && sipm_1 == 0) continue;
        
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
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay/sipms_[%d][%d]_and_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    profile_1->GetXaxis()->SetRangeUser(0,30);
    profile_2->GetXaxis()->SetRangeUser(0,30);
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Low/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,145,-10,135);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,145,-10,135);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    for(int i=706; i<=725; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;
        if(i == 715 && feb_2 == 1 && sipm_2 == 8) continue;
        if(i == 718 && feb_1 == 2 && sipm_1 == 0) continue;
        
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
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted/sipms_[%d][%d]_and_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Scaled plots //
    
    canvas_2 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    profile_3 = new TProfile(*profile_1);
    profile_4 = new TProfile(*profile_2);
    profile_5 = new TProfile(*profile_1);
    profile_6 = new TProfile(*profile_2);
    profile_3->GetXaxis()->SetRangeUser(80,120);
    profile_4->GetXaxis()->SetRangeUser(80,120);
    
    profile_5->Scale(1/profile_3->GetMean(2));
    profile_6->Scale(1/profile_4->GetMean(2));
    
    profile_5->GetYaxis()->SetRangeUser(-0.5,1.5);
    profile_5->GetYaxis()->SetTitle("normalized average PE");
    
    profile_5->Draw();
    profile_6->Draw("SAME");
    legend_1->Draw();
    
    canvas_2->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Normalized/sipms_[%d][%d]_and_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_pe_vs_x_plot_overlay_shifted_low(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,1450,-10,135);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,1450,-10,135);
    profile_1->GetXaxis()->SetRangeUser(-3,10);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    for(int i=706; i<=725; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;
        if(i == 715 && feb_2 == 1 && sipm_2 == 8) continue;
        if(i == 718 && feb_1 == 2 && sipm_1 == 0) continue;
        
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
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Low/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Ratio plots //
    
    canvas_2 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    projection_1 = profile_1->ProjectionX();
    projection_2 = profile_2->ProjectionX();
    projection_2->GetXaxis()->SetRangeUser(-3,22);
    projection_2->GetYaxis()->SetRangeUser(1,2);
    projection_2->GetXaxis()->SetTitle("adjusted x-position (mm)");
    projection_2->GetYaxis()->SetTitle("ratio");
    projection_2->GetXaxis()->SetTitleSize(0.05);
    projection_2->GetYaxis()->SetTitleSize(0.05);
    projection_2->GetYaxis()->SetTitleOffset(0.6);
    projection_2->GetXaxis()->SetTitleOffset(1.2);
    projection_2->GetXaxis()->SetLabelSize(0.06);
    projection_2->GetYaxis()->SetLabelSize(0.06);
    projection_2->GetYaxis()->SetTitleOffset(1.0);
    projection_2->SetLineColor(kGreen);
    projection_2->Divide(projection_1);
    
    /*
    if(feb_1==2 && sipm_1==0)
        f_1 = new TF1("f_1","[0]*x+[1]",5,20);
    else
        f_1 = new TF1("f_1","[0]*x+[1]",1,15);
    */
    
    f_1 = new TF1("f_1","[0]*x+[1]",1,15);
    
    result_1 = projection_2->Fit(f_1, "RS");
    double m = result_1->Parameter(0);
    double b = result_1->Parameter(1);
    
    legend_2 = new TLegend(0.9, 0.9, 0.5, 0.775, "");
    legend_2->SetTextSize(0.04);
    legend_2->AddEntry(projection_2, Form("SiPM [%d][%d]:[%d][%d]",feb_2,sipm_2,feb_1,sipm_1));
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));
    
    projection_2->Draw();
    legend_2->Draw();
    canvas_2->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Low_Divided/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Scaled plots //
    
    canvas_3 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    profile_3 = new TProfile(*profile_1);
    profile_4 = new TProfile(*profile_2);
    profile_5 = new TProfile(*profile_1);
    profile_6 = new TProfile(*profile_2);
    profile_3->GetXaxis()->SetRangeUser(80,120);
    profile_4->GetXaxis()->SetRangeUser(80,120);
    
    profile_5->Scale(1/profile_3->GetMean(2));
    profile_6->Scale(1/profile_4->GetMean(2));
    
    profile_5->GetYaxis()->SetRangeUser(-0.5,1.5);
    profile_5->GetYaxis()->SetTitle("normalized average PE");
    
    profile_5->Draw();
    profile_6->Draw("SAME");
    legend_1->Draw();
    
    canvas_3->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Normalized_Low/sipms_[%d][%d]_and_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_sum_scaled_plot(){
    canvas_1 = new TCanvas("canvas 1","",900,500);
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    /*
    profile_1 = new TProfile("1,0","Sum Plot",1450,-10,135,"s");
    profile_2 = new TProfile("2,0","Sum Plot",1450,-10,135,"s");
    profile_3 = new TProfile("1,1","Sum Plot",1450,-10,135,"s");
    profile_4 = new TProfile("2,1","Sum Plot",1450,-10,135,"s");
    profile_5 = new TProfile("1,8","Sum Plot",1450,-10,135,"s");
    profile_6 = new TProfile("2,8","Sum Plot",1450,-10,135,"s");
    auto profile_7 = new TProfile("1,9","Sum Plot",1450,-10,135,"s");
    auto profile_8 = new TProfile("2,9","Sum Plot",1450,-10,135,"s");
    */
    
    profile_1 = new TProfile("1,0","Sum Plot",1450,-10,135);
    profile_2 = new TProfile("2,0","Sum Plot",1450,-10,135);
    profile_3 = new TProfile("1,1","Sum Plot",1450,-10,135);
    profile_4 = new TProfile("2,1","Sum Plot",1450,-10,135);
    profile_5 = new TProfile("1,8","Sum Plot",1450,-10,135);
    profile_6 = new TProfile("2,8","Sum Plot",1450,-10,135);
    auto profile_7 = new TProfile("1,9","Sum Plot",1450,-10,135);
    auto profile_8 = new TProfile("2,9","Sum Plot",1450,-10,135);
    
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetXaxis()->SetRangeUser(-3,10);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    
    profile_1->SetLineColor(kRed);
    profile_5->SetLineColor(kBlue);
    
    for(int i=706; i<=725; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;
        
        in_File = new TFile(Form("Run_0%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35){
            //if(pos_y > 20 && pos_y < 30){
                profile_1->Fill((Double_t)pos_x-1.3-7.4,(Double_t)pe[0][0],1);
                if(i != 718)
                    profile_2->Fill((Double_t)pos_x-1.3-7.4,(Double_t)pe[1][0],1);
                profile_3->Fill((Double_t)pos_x-1.3-7.4,(Double_t)pe[0][1],1);
                profile_4->Fill((Double_t)pos_x-1.3-7.4,(Double_t)pe[1][1],1);
                if(i != 715)
                    profile_5->Fill((Double_t)pos_x-7.4,(Double_t)pe[0][8],1);
                profile_6->Fill((Double_t)pos_x-7.4,(Double_t)pe[1][8],1);
                profile_7->Fill((Double_t)pos_x-7.4,(Double_t)pe[0][9],1);
                profile_8->Fill((Double_t)pos_x-7.4,(Double_t)pe[1][9],1);
            }
        }
        in_File->Close();
    }
    
    // Scaled plots //
    
    auto profile_1_f = new TProfile(*profile_1);
    auto profile_2_f = new TProfile(*profile_2);
    auto profile_3_f = new TProfile(*profile_3);
    auto profile_4_f = new TProfile(*profile_4);
    auto profile_5_f = new TProfile(*profile_5);
    auto profile_6_f = new TProfile(*profile_6);
    auto profile_7_f = new TProfile(*profile_7);
    auto profile_8_f = new TProfile(*profile_8);
    
    profile_1->GetXaxis()->SetRangeUser(80,120);
    profile_2->GetXaxis()->SetRangeUser(80,120);
    profile_3->GetXaxis()->SetRangeUser(80,120);
    profile_4->GetXaxis()->SetRangeUser(80,120);
    profile_5->GetXaxis()->SetRangeUser(80,120);
    profile_6->GetXaxis()->SetRangeUser(80,120);
    profile_7->GetXaxis()->SetRangeUser(80,120);
    profile_8->GetXaxis()->SetRangeUser(80,120);
    
    profile_1_f->Sumw2();
    profile_2_f->Sumw2();
    profile_3_f->Sumw2();
    profile_4_f->Sumw2();
    profile_5_f->Sumw2();
    profile_6_f->Sumw2();
    profile_7_f->Sumw2();
    profile_8_f->Sumw2();
    
    profile_1_f->Scale(1/profile_1->GetMean(2));
    profile_2_f->Scale(1/profile_2->GetMean(2));
    profile_3_f->Scale(1/profile_3->GetMean(2));
    profile_4_f->Scale(1/profile_4->GetMean(2));
    profile_5_f->Scale(1/profile_5->GetMean(2));
    profile_6_f->Scale(1/profile_6->GetMean(2));
    profile_7_f->Scale(1/profile_7->GetMean(2));
    profile_8_f->Scale(1/profile_8->GetMean(2));
    
    profile_1_f->Add(profile_2_f);
    profile_1_f->Add(profile_3_f);
    profile_1_f->Add(profile_4_f);
    profile_5_f->Add(profile_6_f);
    profile_5_f->Add(profile_7_f);
    profile_5_f->Add(profile_8_f);
    
    profile_1_f->GetYaxis()->SetRangeUser(-0,1);
    profile_1_f->GetYaxis()->SetTitle("normalized average PE");
    
    /*
    profile_1_f->Draw();
    profile_5_f->Draw("SAME");
    
    legend_1 = new TLegend(0.58, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1,"Channels 0,1: Non-Reflective");
    legend_1->AddEntry(profile_5,"Channels 8,9: Reflective");
    legend_1->Draw();
    */
    
    /*
    // Checking error bar
     
    cout << "bin content entries error_calc error_hist" << endl;
    for(int i=100; i<=200; i++){
        double bin_content_1 = (double)profile_1_f->GetBinContent(i);
        double n_entries_1 = (double)profile_1_f->GetBinEntries(i);
        double error_calc_1 = bin_content_1/sqrt(n_entries_1);
        double error_hist_1 = (double)profile_1_f->GetBinError(i);
        
        double bin_content_2 = (double)profile_5_f->GetBinContent(i);
        double n_entries_2 = (double)profile_5_f->GetBinEntries(i);
        double error_calc_2 = bin_content_2/sqrt(n_entries_2);
        double error_hist_2 = (double)profile_5_f->GetBinError(i);
        
        profile_1_f->SetBinError(i,error_calc_1);
        profile_5_f->SetBinError(i,error_calc_2);
        
        cout << i << " " << bin_content_1 << " " << n_entries_1 << " " << error_calc_1 << " " << error_hist_1 << endl;
    }
    */
    
    profile_1_f->Draw();
    profile_5_f->Draw("SAME");
    
    legend_1 = new TLegend(0.38, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1,"Non-Reflective");
    legend_1->AddEntry(profile_5,"Reflective");
    legend_1->Draw();
    
    
    canvas_1->Print("My_Plots/Average_PE_vs_X_Position_Plots/Sum_Scaled/sum_scaled.png","png");
    /*
    TFile* f_out = new TFile("My_Plots/Average_PE_vs_X_Position_Plots/Sum_Scaled/sum_scaled_histograms.root","RECREATE");
    profile_1_f->Write("Non-reflective");
    profile_5_f->Write("Reflective");
    f_out->Close();
    
    // Ratio plots //
    
    canvas_2 = new TCanvas("canvas_2","",900,500);
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    projection_1 = profile_1_f->ProjectionX();
    projection_2 = profile_5_f->ProjectionX();
    projection_2->GetXaxis()->SetRangeUser(-3,22);
    projection_2->GetYaxis()->SetRangeUser(1,2);
    projection_2->GetXaxis()->SetTitle("adjusted x-position (mm)");
    projection_2->GetYaxis()->SetTitle("ratio");
    projection_2->GetXaxis()->SetTitleSize(0.05);
    projection_2->GetYaxis()->SetTitleSize(0.05);
    projection_2->GetYaxis()->SetTitleOffset(0.6);
    projection_2->GetXaxis()->SetTitleOffset(1.2);
    projection_2->GetXaxis()->SetLabelSize(0.06);
    projection_2->GetYaxis()->SetLabelSize(0.06);
    projection_2->GetYaxis()->SetTitleOffset(1.0);
    projection_2->SetLineColor(kGreen);
    projection_2->Divide(projection_1);
    
    f_1 = new TF1("f_1","[0]*x+[1]",1,15);
    
    result_1 = projection_2->Fit(f_1, "RS");
    double m = result_1->Parameter(0);
    double b = result_1->Parameter(1);
    
    legend_2 = new TLegend(0.9,0.9,0.625,0.775, "");
    legend_2->SetTextSize(0.04);
    legend_2->AddEntry(projection_2, "Channels 8+9 : 0+1");
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));

    projection_2->Draw();
    legend_2->Draw();
    //canvas_2->Print("My_Plots/Average_PE_vs_X_Position_Plots/Sum_Scaled/divided_sum_scaled.png","png");
    */
}
