/*
 End study comparing silicone-filled & regular counters.
 
 Uses runs from Configuration 1:

 FEB 1 is at x = 0mm
 FEB 2 is at x = 3000mm

 1) filled with silicone
 [0][0] --- [1][0] 25mm
 [0][1] --- [1][1]
 [0][2] --- [1][2] 75mm
 [0][3] --- [1][3]
 
 2)
 [0][4] --- [1][4]
 [0][5] --- [1][5]
 [0][6] --- [1][6]
 [0][7] --- [1][7]
 
 3) reflector at side A
 [0][8] --- [1][8]
 [0][9] --- [1][9]
 [0][10] --- [1][10]
 [0][11] --- [1][11]
 
 4) radiation damaged SiPMs at side A (ch 1/2: 5e9, ch 3/4: 1e10)
 [0][12] --- [1][12]
 [0][13] --- [1][13]
 [0][14] --- [1][14]
 [0][15] --- [1][15]
 */

# include <string>

using namespace std;

void set_canvas_style();
void make_pe_vs_x_plot(int feb, int sipm);
void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2);
void make_pe_vs_x_plot_middle_run(int feb_1, int sipm_1, int feb_2, int sipm_2, int run_num);
void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2);

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
bool valid;
int pe[2][16];
double x_offset_1, x_offset_2;
TLegend* legend_1; // legends for plots
TLegend* legend_2;
TF1* f_1; // fitting the ratio plot
TFitResultPtr result_1;

void End_Study_Silicone_Configuration_1(){
    /*
     //filled with silicone
     make_pe_vs_x_plot(0,0);
     make_pe_vs_x_plot(0,1);
     make_pe_vs_x_plot(0,2);
     make_pe_vs_x_plot(0,3);
     
     make_pe_vs_x_plot(1,0);
     make_pe_vs_x_plot(1,1);
     make_pe_vs_x_plot(1,2);
     make_pe_vs_x_plot(1,3);
     
     //nothing
     make_pe_vs_x_plot(0,4);
     make_pe_vs_x_plot(0,5);
     make_pe_vs_x_plot(0,6);
     make_pe_vs_x_plot(0,7);
     
     make_pe_vs_x_plot(1,4);
     make_pe_vs_x_plot(1,5);
     make_pe_vs_x_plot(1,6);
     make_pe_vs_x_plot(1,7);
     */
    
    /*
    //filled with silicone vs. nothing
    //25mm
    make_pe_vs_x_plot_overlay(0,0,0,4);
    make_pe_vs_x_plot_overlay(0,1,0,5);
    make_pe_vs_x_plot_overlay(1,0,1,4);
    make_pe_vs_x_plot_overlay(1,1,1,5);
    
    //75mm
    make_pe_vs_x_plot_overlay(0,2,0,6);
    make_pe_vs_x_plot_overlay(0,3,0,7);
    make_pe_vs_x_plot_overlay(1,2,1,6);
    make_pe_vs_x_plot_overlay(1,3,1,7);
    */
    
    //int i = 1006;
    //int k = i+25;
    //int i = 1007;
    //int k = i+23;
    int i = 1008;
    int k = i+21;
    
    /*
    //25mm
    make_pe_vs_x_plot_middle_run(0,0,0,4,k);
    make_pe_vs_x_plot_middle_run(0,1,0,5,k);
    make_pe_vs_x_plot_middle_run(1,0,1,4,k);
    make_pe_vs_x_plot_middle_run(1,1,1,5,k);
    //75mm
    make_pe_vs_x_plot_middle_run(0,2,0,6,i);
    make_pe_vs_x_plot_middle_run(0,3,0,7,i);
    make_pe_vs_x_plot_middle_run(1,2,1,6,i);
    make_pe_vs_x_plot_middle_run(1,3,1,7,i);
    */
    
    make_pe_vs_x_plot_overlay_shifted(0,0,0,4);
    make_pe_vs_x_plot_overlay_shifted(0,1,0,5);
    make_pe_vs_x_plot_overlay_shifted(1,0,1,4);
    make_pe_vs_x_plot_overlay_shifted(1,1,1,5);
    
    //75mm
    make_pe_vs_x_plot_overlay_shifted(0,2,0,6);
    make_pe_vs_x_plot_overlay_shifted(0,3,0,7);
    make_pe_vs_x_plot_overlay_shifted(1,2,1,6);
    make_pe_vs_x_plot_overlay_shifted(1,3,1,7);
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
    profile_1 = new TProfile("profile",title,500,-10,40);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    
    // y = 25mm
    if(sipm%4 <= 1){
        for(int i=1019; i<=1023; i++){
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("valid_",&valid);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            if (nentries < 3000) continue;
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 15 && pos_y < 35)
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb][sipm],1);
            }
            in_File->Close();
        }
    }
    // y = 75mm
    else{
        for(int i=1018; i>=1014; i--){
            
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("valid_",&valid);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            if (nentries < 3000) continue;
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85)
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb][sipm],1);
            }
            in_File->Close();
        }
    }
    
    profile_1->Draw();
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/One_Channel/sipm_[%d][%d].png",feb,sipm),"png");
    
    //profile_1->GetXaxis()->SetRangeUser(0,30);
    //canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/One_Channel_Low/sipm_[%d][%d].png",feb,sipm),"png");
}

void make_pe_vs_x_plot_overlay(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    //profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,140,0,140);
    //profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,140,0,140);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,1400,0,140);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,1400,0,140);
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
    
    if(sipm_1%4 <= 1){
        for(int i=1019; i<=1023; i++){
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 15 && pos_y < 35){
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    else{
        for(int i=1018; i>=1014; i--){
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85){
                    profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay/sipms_[%d][%d]_and_[%d][%d]_part_1.png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    profile_1->GetXaxis()->SetRangeUser(0,30);
    profile_2->GetXaxis()->SetRangeUser(0,30);
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Low/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_pe_vs_x_plot_middle_run(int feb_1, int sipm_1, int feb_2, int sipm_2, int run_num){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    //profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1700,1800);
    //profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1700,1800);
    //profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1450,1550);
    //profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1450,1550);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,100,1200,1300);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,100,1200,1300);
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
    
    in_File = new TFile(Form("Run_%dmerged.root",run_num));
    in_File->GetObject(Form("run%d",run_num),run);
    run->SetBranchAddress("position_x_",&pos_x);
    run->SetBranchAddress("position_y_",&pos_y);
    run->SetBranchAddress("PEs_",pe);
    Int_t nentries = (Int_t)run->GetEntries();
    for(Int_t j=0; j<nentries; j++){
        run->GetEntry(j);
        if(run_num >= 1019 && run_num <= 1033){
            if(pos_y > 15 && pos_y < 35){
                profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1][sipm_1],1);
                profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2][sipm_2],1);
            }
        }
        else if(run_num >= 1002 && run_num <= 1018)
            if(pos_y > 65 && pos_y < 85)
            {
                profile_1->Fill((Double_t)pos_x,(Double_t)pe[feb_1][sipm_1],1);
                profile_2->Fill((Double_t)pos_x,(Double_t)pe[feb_2][sipm_2],1);
            }
    }
    string run_name_1 = run->GetName();
    string run_name_2 = run->GetName();
    run_name_1 = "Run " + run_name_1.substr(3,4);
    in_File->Close();
    
    /*legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
     legend_1->SetTextSize(0.05);
     legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
     legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));*/
    
    legend_1 = new TLegend(0.885, 0.1525, 0.425, 0.355, "");
    legend_1->SetTextSize(0.05);
    legend_1->SetHeader(run_name_1.c_str(),"C");
    legend_1->AddEntry(profile_1, Form("[%d][%d] Mean = %6.5lf",feb_1,sipm_1,(double)profile_1->GetMean(2)));
    legend_1->AddEntry(profile_2, Form("[%d][%d] Mean = %6.5lf",feb_2,sipm_2,(double)profile_2->GetMean(2)));
    legend_1->AddEntry("difference",Form("[%d][%d]-[%d][%d] = %6.5lf",feb_1,sipm_1,feb_2,sipm_2,(double)(profile_1->GetMean(2)-profile_2->GetMean(2))));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay/sipms_[%d][%d]_and_[%d][%d]_%s_part_2.png",feb_1,sipm_1,feb_2,sipm_2,run_name_2.c_str()),"png");
}

void make_pe_vs_x_plot_overlay_shifted(int feb_1, int sipm_1, int feb_2, int sipm_2){
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    title = Form("SiPMs [%d][%d] and [%d][%d]",feb_1,sipm_1,feb_2,sipm_2);
    //profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,150,-10,140);
    //profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,150,-10,140);
    profile_1 = new TProfile(Form("SiPM [%d][%d]",feb_1,sipm_1),title,17000,-10,1600);
    profile_2 = new TProfile(Form("SiPM [%d][%d]",feb_2,sipm_2),title,17000,-10,1600);
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
    
    if(sipm_1 == 0 || sipm_1 == 1){
        x_offset_1 = 3.5;
        x_offset_2 = 4.3;
    }
    else if(sipm_1 == 2 || sipm_1 == 3){
        x_offset_1 = 4.05;
        x_offset_2 = 5;
    }
    
    
    if(sipm_1%4 <= 1){
        //for(int i=1019; i<=1023; i++){
        for(int i=1019; i<=1030; i++){
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 15 && pos_y < 35){
                    profile_1->Fill((Double_t)pos_x-x_offset_1,(Double_t)pe[feb_1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x-x_offset_2,(Double_t)pe[feb_2][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    else{
        //for(int i=1018; i>=1014; i--){
        for(int i=1018; i>=1007; i--){
            in_File = new TFile(Form("Run_%dmerged.root",i));
            in_File->GetObject(Form("run%d",i),run);
            run->SetBranchAddress("position_x_",&pos_x);
            run->SetBranchAddress("position_y_",&pos_y);
            run->SetBranchAddress("PEs_",pe);
            
            Int_t nentries = (Int_t)run->GetEntries();
            for(Int_t j=0; j<nentries; j++){
                run->GetEntry(j);
                if(pos_y > 65 && pos_y < 85){
                    profile_1->Fill((Double_t)pos_x-x_offset_1,(Double_t)pe[feb_1][sipm_1],1);
                    profile_2->Fill((Double_t)pos_x-x_offset_2,(Double_t)pe[feb_2][sipm_2],1);
                }
            }
            in_File->Close();
        }
    }
    
    legend_1 = new TLegend(0.5, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, Form("FEB %d channel %d",feb_1,sipm_1));
    legend_1->AddEntry(profile_2, Form("FEB %d channel %d",feb_2,sipm_2));
    
    profile_1->Draw();
    profile_2->Draw("SAME");
    legend_1->Draw();
    profile_1->GetXaxis()->SetRangeUser(-5,200);
    profile_2->GetXaxis()->SetRangeUser(-5,200);
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
    profile_1->GetXaxis()->SetRangeUser(-5,15);
    profile_2->GetXaxis()->SetRangeUser(-5,15);
    canvas_1->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Low/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
    
     // Ratio plots //
     
     canvas_2 = new TCanvas();
     gStyle->SetTitleSize(0.075,"t");
     gPad->SetLeftMargin(0.125);
     gPad->SetBottomMargin(0.125);
     
     profile_1->GetXaxis()->SetRangeUser(-3,22);
     profile_2->GetXaxis()->SetRangeUser(-3,22);
     
     projection_2 = profile_1->ProjectionX();
     projection_1 = profile_2->ProjectionX();
     projection_2->GetXaxis()->SetRangeUser(-3,22);
     projection_2->GetYaxis()->SetRangeUser(0,2);
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
     canvas_2->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Divided/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
     
     // Scaled plots //
     
     canvas_3 = new TCanvas();
     gStyle->SetTitleSize(0.075,"t");
     gPad->SetLeftMargin(0.125);
     gPad->SetBottomMargin(0.125);
     
     profile_3 = new TProfile(*profile_1);
     profile_4 = new TProfile(*profile_2);
     profile_5 = new TProfile(*profile_1);
     profile_6 = new TProfile(*profile_2);
     profile_3->GetXaxis()->SetRangeUser(1480,1520);
     profile_4->GetXaxis()->SetRangeUser(1480,1520);
     
     profile_5->Scale(1/profile_3->GetMean(2));
     profile_6->Scale(1/profile_4->GetMean(2));
     
     profile_5->GetYaxis()->SetRangeUser(-0.5,1.7);
     profile_5->GetYaxis()->SetTitle("normalized average PE");
     
     profile_5->Draw();
     profile_6->Draw("SAME");
     legend_1->Draw();
     
     canvas_3->Print(Form("Configuration_1_Plots/Average_PE_vs_X_Position_Plots/Two_Channel_Overlay_Shifted_Normalized/sipms_[%d][%d]_and_[%d][%d].png",feb_1,sipm_1,feb_2,sipm_2),"png");
}

void make_sum_scaled_plot(int counter_num){
    canvas_1 = new TCanvas("canvas 1","",900,500);
    set_canvas_style();
    
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
                // counter 2
                profile_1->Fill((Double_t)pos_x-5,(Double_t)pe[0][2],1);
                profile_2->Fill((Double_t)pos_x-5,(Double_t)pe[1][6],1);
                profile_3->Fill((Double_t)pos_x-5,(Double_t)pe[0][3],1);
                profile_4->Fill((Double_t)pos_x-5,(Double_t)pe[1][7],1);
                
                // counter 3
                /*
                 profile_1->Fill((Double_t)pos_x-6,(Double_t)pe[0][6],1);
                 profile_2->Fill((Double_t)pos_x-6,(Double_t)pe[1][10],1);
                 profile_3->Fill((Double_t)pos_x-6,(Double_t)pe[0][7],1);
                 profile_4->Fill((Double_t)pos_x-6,(Double_t)pe[1][11],1);
                 */
                
                profile_5->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][14],1);
                profile_6->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][14],1);
                profile_7->Fill((Double_t)pos_x-5.5,(Double_t)pe[0][15],1);
                profile_8->Fill((Double_t)pos_x-5.5,(Double_t)pe[1][15],1);
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
    
    profile_1->GetXaxis()->SetRangeUser(90,100);
    profile_2->GetXaxis()->SetRangeUser(90,100);
    profile_3->GetXaxis()->SetRangeUser(90,100);
    profile_4->GetXaxis()->SetRangeUser(90,100);
    profile_5->GetXaxis()->SetRangeUser(90,100);
    profile_6->GetXaxis()->SetRangeUser(90,100);
    profile_7->GetXaxis()->SetRangeUser(90,100);
    profile_8->GetXaxis()->SetRangeUser(90,100);
    
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
    
    profile_1_f->Draw();
    profile_5_f->Draw("SAME");
    
    legend_1 = new TLegend(0.38, 0.875, 0.16, 0.75, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(profile_1, "Non-Reflective");
    legend_1->AddEntry(profile_5, "Reflective");
    legend_1->Draw();
    
    canvas_1->Print(Form("Plots/Average_PE_vs_X_Position_Plots_75mm/Sum_Scaled/counter_%d_sum_scaled.png",counter_num),"png");
    
    TFile* f_out = new TFile(Form("Plots/Average_PE_vs_X_Position_Plots_75mm/Sum_Scaled/counter_%d_sum_scaled_histograms.root",counter_num),"RECREATE");
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
    projection_2->GetYaxis()->SetRangeUser(1,3);
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
    legend_2->AddEntry(projection_2, "Reflective : Non-Reflective");
    legend_2->AddEntry(f_1, Form("Fit: y = %5.4lfx+%5.4lf",m,b));
    
    projection_2->Draw();
    legend_2->Draw();
    canvas_2->Print(Form("Plots/Average_PE_vs_X_Position_Plots_75mm/Sum_Scaled/counter_%d_divided_sum_scaled.png",counter_num),"png");
}
