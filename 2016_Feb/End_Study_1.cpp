// x : 2800mm - x(table in mm) = x(beam in mm)
// y : 345mm - y(table in mm) = x(beam in mm)

// creates xy positions plots, x positions plots, and PE distribution plots

# include <string>

using namespace std;

string get_run_name(TTree* run);
void make_position_plot(TTree* run);
void make_x_position_plot(TTree* run);
void make_pe_distribution_plot(TTree* run, int i_feb, int i_sipm);

void End_Study_1(){
    
    for(int i = 706; i <= 725; i++){
        if(i == 710 || i == 712 || i == 714 || i == 723 || i == 724) continue;

        TFile* in_File = new TFile(Form("Run_0%dmerged.root",i));
        TTree* run;
        in_File->GetObject(Form("run%d",i),run);
        
        
        make_position_plot(run);
        make_x_position_plot(run);
        make_pe_distribution_plot(run,1,0);
        make_pe_distribution_plot(run,1,1);
        make_pe_distribution_plot(run,1,8);
        make_pe_distribution_plot(run,1,9);
        make_pe_distribution_plot(run,2,0);
        make_pe_distribution_plot(run,2,1);
        make_pe_distribution_plot(run,2,8);
        make_pe_distribution_plot(run,2,9);
        
        in_File->Close();
    }
}

string get_run_name(TTree* run){
    return run->GetName();
}

void make_position_plot(TTree* run){
    // draws the histogram from data
    auto canvas = new TCanvas();
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    gStyle->SetTitleY(0.975);
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.2);
    gStyle->SetStatY(0.9);
    run->Draw("position_y_:position_x_ >> positions(340,-30,140,250,-20,105)");
    
    // gets the histogram
    auto positions = (TH2F*)gDirectory->FindObjectAny("positions");
    
    // makes the histogram look nice
    string title = "Run "+get_run_name(run).substr(3,3)+" Position of Proton Collisions with Dicounter";
    positions->SetTitle(title.c_str());
    positions->GetXaxis()->SetTitle("x-position (mm)");
    positions->GetXaxis()->SetTitleSize(0.06);
    positions->GetXaxis()->SetLabelSize(0.06);
    positions->GetYaxis()->SetTitle("y-position (mm)");
    positions->GetYaxis()->SetTitleSize(0.06);
    positions->GetYaxis()->SetLabelSize(0.06);
    
    // draws the final histogram
    positions->Draw();
    
    // effectively changes title size
    canvas->Update();
    TPaveText *pt = (TPaveText*)(canvas->GetPrimitive("title"));
    pt->SetTextSize(0.065);
    canvas->Modified();
    
    // saves the histogram as a .png file
    string out_name = "My_Plots/Position_Plots/"+get_run_name(run)+"_position_plot.png";
    canvas->Print(out_name.c_str(), "png");
}

void make_x_position_plot(TTree* run){
    // draws the histogram from data
    auto canvas = new TCanvas();
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    gStyle->SetTitleSize(0.06,"t");
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.2);
    gStyle->SetStatY(0.9);
    run->Draw("position_x_ >> x_positions(130)");
    
    // gets the histogram
    auto x_positions = (TH1F*)gDirectory->FindObjectAny("x_positions");
    
    // makes the histogram look nice
    string title_name = "Run "+get_run_name(run).substr(3,3)+" Distribution of X-Positions of Proton Collisions with Dicounter";
    x_positions->SetTitle(title_name.c_str());
    x_positions->GetXaxis()->SetTitle("x-position (mm)");
    x_positions->GetXaxis()->SetTitleSize(0.05);
    x_positions->GetXaxis()->SetLabelSize(0.05);
    x_positions->GetYaxis()->SetTitle("count");
    x_positions->GetYaxis()->SetTitleOffset(1.2);
    x_positions->GetYaxis()->SetTitleSize(0.05);
    x_positions->GetYaxis()->SetLabelSize(0.05);
    
    // draws the final histogram
    x_positions->Draw();
    
    // effectively changes title size
    canvas->Update();
    TPaveText *pt = (TPaveText*)(canvas->GetPrimitive("title"));
    pt->SetTextSize(0.05);
    canvas->Modified();
    
    string out_name = "My_Plots/X_Position_Plots/"+get_run_name(run)+"_x_position_plot.png";
    canvas->Print(out_name.c_str(), "png");
}

void make_pe_distribution_plot(TTree* run, int i_feb, int i_sipm){
    // draws the histograms from data
    auto canvas = new TCanvas();
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    gStyle->SetTitleSize(0.06,"t");
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.2);
    gStyle->SetStatY(0.9);
    
    string feb = to_string(i_feb);
    string sipm = to_string(i_sipm);
    string draw_option = "PEs_["+to_string(i_feb-1)+"]["+sipm+"] >> pe_distr(250,0,250)";
    run->Draw(draw_option.c_str());
    
    // gets the histograms
    auto pe_distr = (TH1F*)gDirectory->FindObjectAny("pe_distr");
    
    // sets titles
    string title_name = "Run "+get_run_name(run).substr(3,3)+" Channel ["+feb+"]["+sipm+"] PE Distribution";
    pe_distr->SetTitle(title_name.c_str());
    pe_distr->GetXaxis()->SetTitle("# PE");
    pe_distr->GetYaxis()->SetTitle("Counts");
    pe_distr->GetXaxis()->SetTitleSize(0.06);
    pe_distr->GetYaxis()->SetTitleSize(0.06);
    pe_distr->GetXaxis()->SetLabelSize(0.06);
    pe_distr->GetYaxis()->SetLabelSize(0.06);
    pe_distr->GetYaxis()->SetTitleOffset(1.1);
    pe_distr->GetYaxis()->SetRangeUser(0,2000);
    pe_distr->GetYaxis()->SetNdivisions(5,5,0);
    
    // draws the final histograms
    pe_distr->SetName(Form("pe_distr[%d][%d]",i_feb,i_sipm));
    pe_distr->Draw();
    
    // saves the histogram as a .png file
    string out_name = "My_Plots/PE_Distribution_Plots/"+get_run_name(run)+"_sipm_["+feb+"]["+sipm+"]_pe_distribution.png";
    canvas->Print(out_name.c_str());
}
