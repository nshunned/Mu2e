# include <stdio.h>
# include <string>

using namespace std;

int get_run_number(TTree *run);
void make_position_plot(TTree* run);
void make_xy_position_plot(TTree* run);
void make(int i);

void Paper(){
    make(1002);
    //make(1067);
    //make(1291);
    for(int i = 1005; i <= 1018; i++)
        make(i);
}

void make(int i){
    
    TFile* in_File = new TFile(Form("Run_%dmerged.root",i));
    TTree* run;
    in_File->GetObject(Form("run%d",i),run);
        
    make_position_plot(run);
    //make_xy_position_plot(run);
    
}

int get_run_number(TTree *run){
    string s(run->GetName(),3,4);
    return stoi(s);
}

void make_position_plot(TTree* run){
    TCanvas* canvas = new TCanvas();

    run->Draw("position_y_:position_x_ >> positions(2600,2600)");
    TH2F* positions = (TH2F*) gDirectory->FindObjectAny("positions");
    
    string title = "Run "+to_string(get_run_number(run))+" Positions of Proton Collisions with Dicounter";
    positions->SetTitle(title.c_str());
    positions->GetXaxis()->SetTitle("x-position (mm)");
    positions->GetYaxis()->SetTitle("y-position (mm)");
    positions->GetYaxis()->SetTitleOffset(1.4);
    
    positions->Draw();
    string out_name = "Plots/Position_Plots/"+to_string(get_run_number(run))+".png";
    canvas->Print(out_name.c_str(),"png");
}

void make_xy_position_plot(TTree* run){
    TCanvas* canvas = new TCanvas("canvas","",600,1200);
    canvas->Divide(0,2,0.01,0.01);
    gStyle->SetTitleSize(0.06,"t");
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.25);
    gStyle->SetStatY(0.9);
    
    canvas->cd(1);
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    run->Draw("position_x_ >> position_x(130)");
    TH1F* positions_x = (TH1F*) gDirectory->FindObjectAny("position_x");
    TF1* gau_x  = new TF1("gau","gaus");
    positions_x->Fit(gau_x);
    
    canvas->cd(2);
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    run->Draw("position_y_ >> position_y(130)");
    TH1F* positions_y = (TH1F*) gDirectory->FindObjectAny("position_y");
    TF1* gau_y  = new TF1("gau","gaus");
    positions_y->Fit(gau_y);
    
    string title_x = "Run "+to_string(get_run_number(run))+" X-Positions of Proton Collisions";
    positions_x->SetTitle(title_x.c_str());
    positions_x->GetXaxis()->SetTitle("x-position (mm)");
    positions_x->GetXaxis()->SetTitleSize(0.06);
    positions_x->GetXaxis()->SetLabelSize(0.06);
    positions_x->GetYaxis()->SetTitle("count");
    positions_x->GetYaxis()->SetTitleSize(0.06);
    positions_x->GetYaxis()->SetLabelSize(0.06);
    positions_x->GetYaxis()->SetTitleOffset(1.1);
    
    string title_y = "Run "+to_string(get_run_number(run))+" Y-Positions of Proton Collisions";
    positions_y->SetTitle(title_y.c_str());
    positions_y->GetXaxis()->SetTitle("y-position (mm)");
    positions_y->GetXaxis()->SetTitleSize(0.06);
    positions_y->GetXaxis()->SetLabelSize(0.06);
    positions_y->GetYaxis()->SetTitle("count");
    positions_y->GetYaxis()->SetTitleSize(0.06);
    positions_y->GetYaxis()->SetLabelSize(0.06);
    positions_y->GetYaxis()->SetTitleOffset(1.1);
    
    canvas->cd(1);
    positions_x->Draw();
    canvas->cd(2);
    positions_y->Draw();
    
    string out_name = "Plots/XY_Position_Plots/"+to_string(get_run_number(run))+".png";
    canvas->Print(out_name.c_str(),"png");
}
