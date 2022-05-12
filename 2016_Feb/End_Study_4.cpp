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
void make_pe_vs_x_plots_from_single_run(int feb, int sipm);
    
TCanvas* canvas_1;
char* title;
TProfile* profile_1;
TFile* in_File; // file-scanning pointers
TTree* run;
double pos_x, pos_y; // desired variables to be extracted from files
int pe[2][16];

void End_Study_4(){
    make_pe_vs_x_plot(1,8);
    make_pe_vs_x_plots_from_single_run(1,8);
    make_pe_vs_x_plot(2,0);
    make_pe_vs_x_plots_from_single_run(2,0);

}

void make_pe_vs_x_plot(int feb, int sipm){
    canvas_1 = new TCanvas();
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);

    title = Form("FEB %d Channel %d",feb,sipm);
    profile_1 = new TProfile("profile",title,650,-5,60);
    profile_1->GetXaxis()->SetTitle("x-position (mm)");
    profile_1->GetYaxis()->SetTitle("average PE");
    profile_1->GetYaxis()->SetRangeUser(-20,150);
    profile_1->GetXaxis()->SetTitleSize(0.05);
    profile_1->GetYaxis()->SetTitleSize(0.05);
    profile_1->GetXaxis()->SetTitleOffset(1.2);
    profile_1->GetYaxis()->SetTitleOffset(1.2);
    profile_1->GetXaxis()->SetLabelSize(0.06);
    profile_1->GetYaxis()->SetLabelSize(0.06);
    
    for(int i=711; i<=725; i++){
        if(i==712 || i==714 || i == 723 || i == 724) continue;
        
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
                profile_1->Fill((Double_t)pos_x-7.4,(Double_t)pe[feb-1][sipm],1);
        }
        in_File->Close();
    }
    
    profile_1->Draw();
    canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Dip_Investigation/sipm_[%d][%d]_all_runs.png",feb,sipm));
}

void make_pe_vs_x_plots_from_single_run(int feb, int sipm){
    
    for(int i=711; i<=725; i++){
        if(i==712 || i==714 || i == 723 || i == 724) continue;
        
        canvas_1 = new TCanvas(Form("run%d",i),"",1);
        gStyle->SetTitleSize(0.075,"t");
        gPad->SetLeftMargin(0.125);
        gPad->SetBottomMargin(0.125);
        
        title = Form("Run %d FEB %d Channel %d",i,feb,sipm);
        profile_1 = new TProfile("profile",title,650,-5,60);
        profile_1->GetXaxis()->SetTitle("x-position (mm)");
        profile_1->GetYaxis()->SetTitle("average PE");
        profile_1->GetYaxis()->SetRangeUser(-20,150);
        profile_1->GetXaxis()->SetTitleSize(0.05);
        profile_1->GetYaxis()->SetTitleSize(0.05);
        profile_1->GetXaxis()->SetTitleOffset(1.2);
        profile_1->GetYaxis()->SetTitleOffset(1.2);
        profile_1->GetXaxis()->SetLabelSize(0.06);
        profile_1->GetYaxis()->SetLabelSize(0.06);
        
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
                profile_1->Fill((Double_t)pos_x-7.4,(Double_t)pe[feb-1][sipm],1);
        }
        profile_1->Draw();
        canvas_1->Print(Form("My_Plots/Average_PE_vs_X_Position_Plots/Dip_Investigation/sipm_[%d][%d]_run%d.png",feb,sipm,i));
        
        in_File->Close();
    }
}

