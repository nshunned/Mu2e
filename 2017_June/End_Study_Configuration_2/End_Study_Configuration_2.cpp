/*
 FEB 1 is at x = 0mm
 FEB 2 is at x = 3000mm
 
 1) filled with silicone, reflector at side A (FEB 1)
 [0][8] --- [1][0] 25mm
 [0][9] --- [1][1]
 [0][10] --- [1][2] 75mm
 [0][11] --- [1][3]
 
 2) optical grease at both sides
 [0][0] --- [1][4]
 [0][1] --- [1][5]
 [0][2] --- [1][6]
 [0][3] --- [1][7]
 
 3) default
 [0][4] --- [1][8]
 [0][5] --- [1][9]
 [0][6] --- [1][10]
 [0][7] --- [1][11]
 
 4) white paint
 [0][12] --- [1][12]
 [0][13] --- [1][13]
 [0][14] --- [1][14]
 [0][15] --- [1][15]
 */

# include <string>

using namespace std;

void set_canvas_style();
void make_pe_vs_x_plot(int feb, int sipm);
void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2); // 1 nonreflective; 2 reflective
void make_pe_vs_x_plot_middle_run(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_pe_vs_x_plot_overlay_shifted_low(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_sum_scaled_plot_separated();
void make_sum_scaled_plot();

TCanvas* canvas_1;
TCanvas* canvas_2;
TCanvas* canvas_3;
const char* title;
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

void End_Study_Configuration_2(){
    /*
    // bare ends
    make_pe_vs_x_plot(1,4);
    make_pe_vs_x_plot(1,5);
    make_pe_vs_x_plot(1,6);
    make_pe_vs_x_plot(1,7);
    make_pe_vs_x_plot(2,8);
    make_pe_vs_x_plot(2,9);
    make_pe_vs_x_plot(2,10);
    make_pe_vs_x_plot(2,11);
     
    // white ends
    make_pe_vs_x_plot(1,12);
    make_pe_vs_x_plot(1,13);
    make_pe_vs_x_plot(1,14);
    make_pe_vs_x_plot(1,15);
    make_pe_vs_x_plot(2,12);
    make_pe_vs_x_plot(2,13);
    make_pe_vs_x_plot(2,14);
    make_pe_vs_x_plot(2,15);
    
    make_pe_vs_x_plot_overlay(1,4,1,12);
    make_pe_vs_x_plot_overlay(1,5,1,13);
    make_pe_vs_x_plot_overlay(1,6,1,14);
    make_pe_vs_x_plot_overlay(1,7,1,15);
    
    make_pe_vs_x_plot_overlay(2,8,2,12);
    make_pe_vs_x_plot_overlay(2,9,2,13);
    make_pe_vs_x_plot_overlay(2,10,2,14);
    make_pe_vs_x_plot_overlay(2,11,2,15);
    
    make_pe_vs_x_plot_middle_run(1,4,1,12);
    make_pe_vs_x_plot_middle_run(1,5,1,13);
    make_pe_vs_x_plot_middle_run(1,6,1,14);
    make_pe_vs_x_plot_middle_run(1,7,1,15);
    make_pe_vs_x_plot_middle_run(2,8,2,12);
    make_pe_vs_x_plot_middle_run(2,9,2,13);
    make_pe_vs_x_plot_middle_run(2,10,2,14);
    make_pe_vs_x_plot_middle_run(2,11,2,15);
    
    make_pe_vs_x_plot_overlay_shifted(1,4,1,12);
    make_pe_vs_x_plot_overlay_shifted(1,5,1,13);
    make_pe_vs_x_plot_overlay_shifted(1,6,1,14);
    make_pe_vs_x_plot_overlay_shifted(1,7,1,15);
    make_pe_vs_x_plot_overlay_shifted(2,8,2,12);
    make_pe_vs_x_plot_overlay_shifted(2,9,2,13);
    make_pe_vs_x_plot_overlay_shifted(2,10,2,14);
    make_pe_vs_x_plot_overlay_shifted(2,11,2,15);
    
    make_pe_vs_x_plot_overlay_shifted_low(1,4,1,12);
    make_pe_vs_x_plot_overlay_shifted_low(1,5,1,13);
    make_pe_vs_x_plot_overlay_shifted_low(1,6,1,14);
    make_pe_vs_x_plot_overlay_shifted_low(1,7,1,15);
    make_pe_vs_x_plot_overlay_shifted_low(2,8,2,12);
    make_pe_vs_x_plot_overlay_shifted_low(2,9,2,13);
    make_pe_vs_x_plot_overlay_shifted_low(2,10,2,14);
    make_pe_vs_x_plot_overlay_shifted_low(2,11,2,15);
    */
    
    make_sum_scaled_plot_separated();
    
    //make_sum_scaled_plot();
}

void set_canvas_style(){
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
}

void make_pe_vs_x_plot(int feb, int sipm){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("FEB %d Channel %d",feb,sipm);
    profile_1 = new TProfile("profile",title,400,0,40);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("response");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    
    gSystem->cd("../");
    
    // y = 25mm
    if(sipm%4 <= 1) {
        for(int i=1044; i<=1050; i++){
            if(i==1050) i = 1068;
            
            // gets the tree for run i
            in_File = new TFile(Form("Run_%dmerged.root",i));
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
    }
    // y = 75mm
    else {
        for(int i=1050; i<=1055; i++){
            if(i==1055) i = 1069;
            
            // gets the tree for run i
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            // "branches" these variables in the tree
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            // fills the profile
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85)
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb-1][sipm],1);
            }
            in_File->Close();
        }
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    profile_1->Draw();
    canvas_1->Print(Form("One_Channel/[%d][%d].png",feb,sipm),"png");
    
    profile_1->GetXaxis()->SetRangeUser(0,30);
    canvas_1->Print(Form("One_Channel_Low/[%d][%d].png",feb,sipm),"png");
}

void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,140,0,140);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,140,0,140);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("response");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    gSystem->cd("../");
    
    // y = 25mm
    if(sipm_1%4 <= 1) {
        for(int i=1044; i<=1050; i++){
            if(i==1050) i = 1068;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
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
    }
    // y = 75mm
    else {
        for(int i=1050; i<=1055; i++){
            if(i==1055) i = 1069;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85){
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1-1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2-1][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("Two_Channel_Overlay/[%d][%d]_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    profile_1->GetXaxis()->SetRangeUser(0,30);
    profile_2->GetXaxis()->SetRangeUser(0,30);
    canvas_1->Print(Form("Two_Channel_Overlay_Low/[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_pe_vs_x_plot_middle_run(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    
    if(sipm_1%4 <= 1) {
        profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1450,1550);
        profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1450,1550);
    }
    else {
        profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1200,1300);
        profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1200,1300);
    }
    
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("response");
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
    
    gSystem->cd("../");
    
    // y = 25mm
    if(sipm_1%4 <= 1) {
        int i = 1038;
        in_File = new TFile(Form("Run_%dmerged.root",i));
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
    }
    // y = 75mm
    else {
        int i = 1062;
        in_File = new TFile(Form("Run_%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 65 && pos_y < 85){
                profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1-1][sipm_1],1);
                profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2-1][sipm_2],1);
            }
        }
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    string run_name_1 = run->GetName();
    string run_name_2 = run->GetName();
    run_name_1 = "Run " + run_name_1.substr(3,4);
    in_File->Close();
    
    legend_1 = new TLegend(0.885, 0.1525, 0.425, 0.355, "");
    legend_1->SetTextSize(0.05);
    legend_1->SetHeader(run_name_1.c_str(),"C");
    legend_1->AddEntry(profile_1, Form("[%d][%d] Mean = %6.5lf",feb_1,sipm_1,(double)profile_1->GetMean(2)));
    legend_1->AddEntry(profile_2, Form("[%d][%d] Mean = %6.5lf",feb_2,sipm_2,(double)profile_2->GetMean(2)));
    legend_1->AddEntry("difference",Form("[%d][%d]-[%d][%d] = %6.5lf",feb_1,sipm_1,feb_2,sipm_2,(double)(profile_1->GetMean(2)-profile_2->GetMean(2))));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("Two_Channel_Overlay/[%d][%d]_[%d][%d]_%s_part_2.png",feb_1,sipm_1,feb_2,sipm_2,run_name_2.c_str()),"png");
}

void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,145,-10,135);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,145,-10,135);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetYaxis()->SetTitle("response");
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    gSystem->cd("../");
    
    // y = 25mm
    if(sipm_1%4 <= 1) {
        for(int i=1044; i<=1050; i++){
            if(i==1050) i = 1068;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 15 && pos_y < 35){
                    //profile_1->Fill((Double_t)pos_x-4.8,(Double_t)pe[feb_1-1][sipm_1],1); // counter 2
                    profile_1->Fill((Double_t)pos_x-5.6,(Double_t)pe[feb_1-1][sipm_1],1); // counter 3
                    profile_2->Fill((Double_t)pos_x-6.1,(Double_t)pe[feb_2-1][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    // y = 75mm
    else {
        for(int i=1050; i<=1055; i++){
            if(i==1055) i = 1069;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85){
                    //profile_1->Fill((Double_t)pos_x-5,(Double_t)pe[feb_1-1][sipm_1],1); // counter 2
                    profile_1->Fill((Double_t)pos_x-6,(Double_t)pe[feb_1-1][sipm_1],1); // counter 3
                    profile_2->Fill((Double_t)pos_x-5.5,(Double_t)pe[feb_2-1][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    
    canvas_1->Print(Form("Two_Channel_Overlay_Shifted/[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Scaled plots //
    
    canvas_2 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    profile_3 = new TProfile(*profile_1);
    profile_4 = new TProfile(*profile_2);
    profile_5 = new TProfile(*profile_1);
    profile_6 = new TProfile(*profile_2);
    profile_3->GetXaxis()->SetRangeUser(90,100);
    profile_4->GetXaxis()->SetRangeUser(90,100);
    
    profile_5->Scale(1/profile_3->GetMean(2));
    profile_6->Scale(1/profile_4->GetMean(2));
    
    profile_5->GetYaxis()->SetRangeUser(-0.5,1.7);
    profile_5->GetYaxis()->SetTitle("normalized average response");
    
    profile_5->Draw();
    profile_6->Draw("SAME");
    legend_1->Draw();
    
    canvas_2->Print(Form("Two_Channel_Overlay_Shifted_Normalized/[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_pe_vs_x_plot_overlay_shifted_low(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,1450,-10,135);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,1450,-10,135);
    profile_1->GetXaxis()->SetRangeUser(-3,10);
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1->GetYaxis()->SetTitle("response");
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    profile_1->SetLineColor(kRed);
    profile_2->SetLineColor(kBlue);
    
    gSystem->cd("../");
    
    // y = 25mm
    if(sipm_1%4 <= 1) {
        for(int i=1044; i<=1050; i++){
            if(i==1050) i = 1068;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 15 && pos_y < 35){
                    profile_1->Fill((Double_t)pos_x-5.6,(Double_t)pe[feb_1-1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x-6.1,(Double_t)pe[feb_2-1][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    // y = 75mm
    else {
        for(int i=1050; i<=1055; i++){
            if(i==1055) i = 1069;
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85){
                    profile_1->Fill((Double_t)pos_x-6,(Double_t)pe[feb_1-1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x-5.5,(Double_t)pe[feb_2-1][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    
    canvas_1->Print(Form("Two_Channel_Overlay_Shifted_Low/sipms_[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Ratio plots //
    
    canvas_2 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    projection_1 = profile_1->ProjectionX();
    projection_2 = profile_2->ProjectionX();
    projection_2->GetXaxis()->SetRangeUser(-3,22);
    projection_2->GetYaxis()->SetRangeUser(1,4);
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
    
    legend_2 = new TLegend(0.9, 0.9, 0.5, 0.775, "");
    legend_2->SetTextSize(0.04);
    legend_2->AddEntry(projection_2, Form("SiPM [%d][%d]:[%d][%d]",feb_2,sipm_2,feb_1,sipm_1));
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));
    
    projection_2->Draw();
    legend_2->Draw();
    canvas_2->Print(Form("Two_Channel_Overlay_Shifted_Low_Divided/[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    // Scaled plots //
    
    canvas_3 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
    
    profile_3 = new TProfile(*profile_1);
    profile_4 = new TProfile(*profile_2);
    profile_5 = new TProfile(*profile_1);
    profile_6 = new TProfile(*profile_2);
    profile_3->GetXaxis()->SetRangeUser(90,100);
    profile_4->GetXaxis()->SetRangeUser(90,100);
    
    profile_5->Scale(1/profile_3->GetMean(2));
    profile_6->Scale(1/profile_4->GetMean(2));
    
    profile_5->GetYaxis()->SetRangeUser(-0.5,1.7);
    profile_5->GetYaxis()->SetTitle("normalized response");
    
    profile_5->Draw();
    profile_6->Draw("SAME");
    legend_1->Draw();
    
    canvas_3->Print(Form("Two_Channel_Overlay_Shifted_Normalized_Low/[%d][%d]_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
}

void make_sum_scaled_plot(){
    canvas_1 = new TCanvas("canvas 1","",900,500);
    set_canvas_style();
    
    // bare ends
    auto profile_1_4 = new TProfile("1,4","Sum Plot",1450,-10,135);
    auto profile_1_5 = new TProfile("1,5","Sum Plot",1450,-10,135);
    auto profile_1_6 = new TProfile("1,6","Sum Plot",1450,-10,135);
    auto profile_1_7 = new TProfile("1,7","Sum Plot",1450,-10,135);
    auto profile_2_8 = new TProfile("2,8","Sum Plot",1450,-10,135);
    auto profile_2_9 = new TProfile("2,9","Sum Plot",1450,-10,135);
    auto profile_2_10 = new TProfile("2,10","Sum Plot",1450,-10,135);
    auto profile_2_11 = new TProfile("2,11","Sum Plot",1450,-10,135);
    
    // white ends
    auto profile_1_12 = new TProfile("1,12","Sum Plot",1450,-10,135);
    auto profile_1_13 = new TProfile("1,13","Sum Plot",1450,-10,135);
    auto profile_1_14 = new TProfile("1,14","Sum Plot",1450,-10,135);
    auto profile_1_15 = new TProfile("1,15","Sum Plot",1450,-10,135);
    auto profile_2_12 = new TProfile("2,12","Sum Plot",1450,-10,135);
    auto profile_2_13 = new TProfile("2,13","Sum Plot",1450,-10,135);
    auto profile_2_14 = new TProfile("2,14","Sum Plot",1450,-10,135);
    auto profile_2_15 = new TProfile("2,15","Sum Plot",1450,-10,135);
    
    TProfile* profiles[16] = {profile_1_4, profile_1_5, profile_1_6, profile_1_7,
                         profile_2_8, profile_2_9, profile_2_10, profile_2_11,
                         profile_1_12, profile_1_13, profile_1_14, profile_1_15,
                         profile_2_12, profile_2_13, profile_2_14, profile_2_15};
    
    profile_1_4->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1_4->GetXaxis()->SetLabelSize(0.06);
    profile_1_4->GetYaxis()->SetLabelSize(0.06);
    profile_1_4->GetXaxis()->SetTitleSize(0.05);
    profile_1_4->GetYaxis()->SetTitleSize(0.05);
    profile_1_4->GetXaxis()->SetTitleOffset(1.2);
    profile_1_4->GetYaxis()->SetTitleOffset(1.2);
    
    profile_1_4->SetLineColor(kRed);
    profile_1_12->SetLineColor(kBlue);
    
    gSystem->cd("../");
    
    // y = 25mm
    for(int i=1044; i<=1050; i++) {
        if(i==1050) i = 1068;
        
        in_File = new TFile(Form("Run_%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35){
                // counter 3
                profile_1_4->Fill((Double_t)pos_x-5.6,(Double_t)pe[0][4],1);
                profile_1_5->Fill((Double_t)pos_x-5.6,(Double_t)pe[0][5],1);
    
                profile_2_8->Fill((Double_t)pos_x-5.6,(Double_t)pe[1][8],1);
                profile_2_9->Fill((Double_t)pos_x-5.6,(Double_t)pe[1][9],1);
                
                // counter 4
                profile_1_12->Fill((Double_t)pos_x-6.1,(Double_t)pe[0][12],1);
                profile_1_13->Fill((Double_t)pos_x-6.1,(Double_t)pe[0][13],1);
                
                profile_2_12->Fill((Double_t)pos_x-6.1,(Double_t)pe[1][12],1);
                profile_2_13->Fill((Double_t)pos_x-6.1,(Double_t)pe[1][13],1);
            }
        }
        in_File->Close();
    }
    
    // y = 75mm
    for(int i=1050; i<=1055; i++) {
        if(i==1055) i = 1069;
        
        in_File = new TFile(Form("Run_%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 65 && pos_y < 85){
                // counter 3
                profile_1_6->Fill((Double_t)pos_x-6,(Double_t)pe[0][6],1);
                profile_1_7->Fill((Double_t)pos_x-6,(Double_t)pe[0][7],1);
                
                profile_2_10->Fill((Double_t)pos_x-6,(Double_t)pe[1][10],1);
                profile_2_11->Fill((Double_t)pos_x-6,(Double_t)pe[1][11],1);
                
                // counter 4
                profile_1_14->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][14],1);
                profile_1_15->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][15],1);
                
                profile_2_14->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][14],1);
                profile_2_15->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][15],1);
            }
        }
        in_File->Close();
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    for(int i = 0; i < 16; i++) {
        profiles[i]->GetXaxis()->SetRangeUser(90,100);
        profiles[i]->Scale(1/profiles[i]->GetMean(2));
    }
    for(int i = 1; i < 8; i++) {
        cout << i << ": " << profile_1_4->Add(profiles[i]) << "       ";
        cout << i+8 << ": " << profile_1_12->Add(profiles[i+8]) << endl;
    }
    profile_1_4->GetXaxis()->SetRangeUser(-3,10);
    profile_1_4->GetYaxis()->SetRangeUser(0,1);
    profile_1_4->GetYaxis()->SetTitle("normalized response");
    profile_1_12->GetXaxis()->SetRangeUser(-3,10);
    
    profile_1_4->Draw();
    profile_1_12->Draw("SAME");
    
    legend_1 = new TLegend(0.38, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1_4, "Default");
    legend_1->AddEntry(profile_1_12, "White Paint");
    legend_1->Draw();
    
    canvas_1->Print("Sum_Scaled/sum_scaled.png","png");
    
    TFile* f_out = new TFile("Sum_Scaled/sum_scaled_histograms.root","RECREATE");
    profile_1_4->Write("Default");
    profile_1_12->Write("White_Paint");
    f_out->Close();
    
    
    // Ratio plots //
    
    canvas_2 = new TCanvas("canvas_2","",900,500);
    set_canvas_style();
    
    projection_1 = profile_1_4->ProjectionX();
    projection_2 = profile_1_12->ProjectionX();
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
    
    legend_2 = new TLegend(0.9,0.9,0.615,0.775, "");
    legend_2->SetTextSize(0.04);
    legend_2->AddEntry(projection_2, "Default : White Paint");
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));
    
    projection_2->Draw();
    legend_2->Draw();
    canvas_2->Print("Sum_Scaled/divided_sum_scaled.png","png");
}

void make_sum_scaled_plot_separated(){
    // default
    auto profile_1_4 = new TProfile("1,4","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_5 = new TProfile("1,5","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_6 = new TProfile("1,6","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_7 = new TProfile("1,7","Sum Plot FEB 1",1450,-10,135);
    auto profile_2_8 = new TProfile("2,8","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_9 = new TProfile("2,9","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_10 = new TProfile("2,10","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_11 = new TProfile("2,11","Sum Plot FEB 2",1450,-10,135);
    
    // white painted
    auto profile_1_12 = new TProfile("1,12","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_13 = new TProfile("1,13","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_14 = new TProfile("1,14","Sum Plot FEB 1",1450,-10,135);
    auto profile_1_15 = new TProfile("1,15","Sum Plot FEB 1",1450,-10,135);
    auto profile_2_12 = new TProfile("2,12","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_13 = new TProfile("2,13","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_14 = new TProfile("2,14","Sum Plot FEB 2",1450,-10,135);
    auto profile_2_15 = new TProfile("2,15","Sum Plot FEB 2",1450,-10,135);
    
    TProfile* profiles[16] = {profile_1_4, profile_1_5, profile_1_6, profile_1_7,
        profile_2_8, profile_2_9, profile_2_10, profile_2_11,
        profile_1_12, profile_1_13, profile_1_14, profile_1_15,
        profile_2_12, profile_2_13, profile_2_14, profile_2_15};
    
    // FEB 1
    profile_1_4->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_1_4->GetXaxis()->SetLabelSize(0.06);
    profile_1_4->GetYaxis()->SetLabelSize(0.06);
    profile_1_4->GetXaxis()->SetTitleSize(0.05);
    profile_1_4->GetYaxis()->SetTitleSize(0.05);
    profile_1_4->GetXaxis()->SetTitleOffset(1.2);
    profile_1_4->GetYaxis()->SetTitleOffset(1.2);
    
    profile_1_4->SetLineColor(kRed);
    profile_1_12->SetLineColor(kBlue);
    
    // FEB 2
    profile_2_8->GetXaxis()->SetTitle("adjusted x-position (mm)");
    profile_2_8->GetXaxis()->SetLabelSize(0.06);
    profile_2_8->GetYaxis()->SetLabelSize(0.06);
    profile_2_8->GetXaxis()->SetTitleSize(0.05);
    profile_2_8->GetYaxis()->SetTitleSize(0.05);
    profile_2_8->GetXaxis()->SetTitleOffset(1.2);
    profile_2_8->GetYaxis()->SetTitleOffset(1.2);
    
    profile_2_8->SetLineColor(kRed);
    profile_2_12->SetLineColor(kBlue);
    
    gSystem->cd("../");
    
    // y = 25mm
    for(int i=1044; i<=1050; i++) {
        if(i==1050) i = 1068;
        
        in_File = new TFile(Form("Run_%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 15 && pos_y < 35){
                // counter 3
                profile_1_4->Fill((Double_t)pos_x-5.6,(Double_t)pe[0][4],1);
                profile_1_5->Fill((Double_t)pos_x-5.6,(Double_t)pe[0][5],1);
                
                profile_2_8->Fill((Double_t)pos_x-5.6,(Double_t)pe[1][8],1);
                profile_2_9->Fill((Double_t)pos_x-5.6,(Double_t)pe[1][9],1);
                
                // counter 4
                profile_1_12->Fill((Double_t)pos_x-6.1,(Double_t)pe[0][12],1);
                profile_1_13->Fill((Double_t)pos_x-6.1,(Double_t)pe[0][13],1);
                
                profile_2_12->Fill((Double_t)pos_x-6.1,(Double_t)pe[1][12],1);
                profile_2_13->Fill((Double_t)pos_x-6.1,(Double_t)pe[1][13],1);
            }
        }
        in_File->Close();
    }
    
    // y = 75mm
    for(int i=1050; i<=1055; i++) {
        if(i==1055) i = 1069;
        
        in_File = new TFile(Form("Run_%dmerged.root",i));
        in_File->GetObject(Form("run%d",i),run);
        run->SetBranchAddress("position_x_",&pos_x);
        run->SetBranchAddress("position_y_",&pos_y);
        run->SetBranchAddress("PEs_",pe);
        
        Int_t nentries = (Int_t)run->GetEntries();
        for(Int_t j=0; j<nentries; j++){
            run->GetEntry(j);
            if(pos_y > 65 && pos_y < 85){
                // counter 3
                profile_1_6->Fill((Double_t)pos_x-6,(Double_t)pe[0][6],1);
                profile_1_7->Fill((Double_t)pos_x-6,(Double_t)pe[0][7],1);
                
                profile_2_10->Fill((Double_t)pos_x-6,(Double_t)pe[1][10],1);
                profile_2_11->Fill((Double_t)pos_x-6,(Double_t)pe[1][11],1);
                
                // counter 4
                profile_1_14->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][14],1);
                profile_1_15->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][15],1);
                
                profile_2_14->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][14],1);
                profile_2_15->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][15],1);
            }
        }
        in_File->Close();
    }
    
    gSystem->cd("End_Study_Configuration_2");
    
    for(int i = 0; i < 16; i++) {
        profiles[i]->GetXaxis()->SetRangeUser(90,100);
        profiles[i]->Scale(1/profiles[i]->GetMean(2));
    }
    for(int i = 1; i < 4; i++) {
        cout << profile_1_4->Add(profiles[i]);
        profile_2_8->Add(profiles[i+4]);
        profile_1_12->Add(profiles[i+8]);
        profile_2_12->Add(profiles[i+12]);
    }

    // FEB 1
    canvas_1 = new TCanvas("canvas 1","",900,500);
    set_canvas_style();
    
    profile_1_4->GetXaxis()->SetRangeUser(-3,10);
    profile_1_4->GetYaxis()->SetRangeUser(0,1);
    profile_1_4->GetYaxis()->SetTitle("normalized response");
    profile_1_12->GetXaxis()->SetRangeUser(-3,10);
    
    profile_1_4->Draw();
    profile_1_12->Draw("SAME");
    
    legend_1 = new TLegend(0.38, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1_4, "Default");
    legend_1->AddEntry(profile_1_12, "White Paint");
    legend_1->Draw();
    
    canvas_1->Print("Sum_Scaled/sum_scaled_feb_1.png","png");
    
    TFile* f_out = new TFile("Sum_Scaled/sum_scaled_feb_1_histograms.root","RECREATE");
    profile_1_4->Write("Default");
    profile_1_12->Write("White_Paint");
    f_out->Close();
    
    // FEB 2
    canvas_2 = new TCanvas("canvas 2","",900,500);
    set_canvas_style();
    
    profile_2_8->GetXaxis()->SetRangeUser(-3,10);
    profile_2_8->GetYaxis()->SetRangeUser(0,1);
    profile_2_8->GetYaxis()->SetTitle("normalized response");
    profile_2_12->GetXaxis()->SetRangeUser(-3,10);
    
    profile_2_8->Draw();
    profile_2_12->Draw("SAME");
    
    legend_2 = new TLegend(0.38, 0.875, 0.16, 0.75, "");
    legend_2->SetTextSize(0.05);
    legend_2->AddEntry(profile_2_8, "Default");
    legend_2->AddEntry(profile_2_12, "White Paint");
    legend_2->Draw();
    
    canvas_2->Print("Sum_Scaled/sum_scaled_feb_2.png","png");
    
    f_out = new TFile("Sum_Scaled/sum_scaled_feb_2_histograms.root","RECREATE");
    profile_2_8->Write("Default");
    profile_2_12->Write("White_Paint");
    f_out->Close();
    
    /*
    // Ratio plots //
    
    canvas_2 = new TCanvas("canvas_2","",900,500);
    set_canvas_style();
    
    projection_1 = profile_1_4->ProjectionX();
    projection_2 = profile_1_12->ProjectionX();
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
    
    legend_2 = new TLegend(0.9,0.9,0.615,0.775, "");
    legend_2->SetTextSize(0.04);
    legend_2->AddEntry(projection_2, "Default : White Paint");
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));
    
    projection_2->Draw();
    legend_2->Draw();
    canvas_2->Print("Sum_Scaled/divided_sum_scaled.png","png");
     */
}
