# include <stdio.h>
# include <string>

using namespace std;

string get_run_name(TTree* run);
void set_canvas_style();
double make_x_position_plot(TTree* run, int option=0);
void make_offset(TTree* middle_run, int sipm, int option=0);
void make_offset_PEs(TTree* middle_run, int sipm, int option=0);
void make_offset_PulseWidth(TTree* middle_run, int sipm, int option=0);
double make_x_position_plot_from_time(TTree* run, int sipm, int option=0);
double make_x_position_plot_from_LEtime(TTree* run, int sipm, int option=0);
double make_x_position_plot_from_pe(TTree* run, int sipm, int option=0);
void make_calculated_vs_measured_plot(double* measured, double* calculated, const char *title);

double time_[2][16];
double LEtime_[2][16];
double x_position_measured[15];
double x_position_from_time[15];
double x_position_from_LEtime[15];
double x_position_from_PEs[15];
string title_name;
string out_name;
TCanvas* canvas_1;
TCanvas* canvas_2;
TCanvas* canvas_3;
TCanvas* canvas_4;
TH1F* time_offset;
TH1F* LEtime_offset;
TH1F* PEs_offset_1;
TH1F* PEs_offset_2;
TH1F* x_position_1;
TH1F* x_position_2;
TGraphErrors* graph_1;
TF1* f_1;
TF1* f_2;
TFitResultPtr result_1;
TFitResultPtr result_2;
TLegend* legend_1;
TLegend* legend_2;


const double V_LIGHT_PEAK = 166.975; //299.792458*10/16;;
const double V_LIGHT_LE = 140.835;

void X_Position_Plots(){
    TTree* middle_run;
    TFile* file_offset = new TFile("Run_1007merged.root");
    file_offset->GetObject("run1007",middle_run);
    
    make_offset(middle_run,2);
    //make_offset_PEs(middle_run,2,1);
    //make_offset_PulseWidth(middle_run,2,1);
    
    TTree* run;
    TFile* file_run;
    int j = 0;
    for(int i=1002; i<=1018; i++){
        if(i==1003) i+=2;
        file_run = new TFile(Form("Run_%dmerged.root",i));
        file_run->GetObject(Form("run%d",i),run);
        
        x_position_measured[j] = make_x_position_plot(run);
        //x_position_from_time[j] = make_x_position_plot_from_time(run,2,1);
        //x_position_from_LEtime[j] = make_x_position_plot_from_LEtime(run,2,1);
        x_position_from_PEs[j] = make_x_position_plot_from_pe(run,2,1);
        
        j++;
        //break;
    }
    //make_calculated_vs_measured_plot(x_position_measured, x_position_from_time, "Time-Calculated vs. Measured X-Position");
    //make_calculated_vs_measured_plot(x_position_measured, x_position_from_LEtime, "LETime-Calculated vs. Measured X-Position");
    make_calculated_vs_measured_plot(x_position_measured, x_position_from_PEs, "PE-Calculated vs. Measured X-Position");
    //make_calculated_vs_measured_plot(x_position_measured, x_position_from_PEs, "Unadjusted PE-Calculated vs. Measured X-Position");
}

string get_run_name(TTree* run){
    return run->GetName();
}

void set_canvas_style(){
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.2);
    gStyle->SetStatY(0.9);
    gStyle->SetTitleAlign(35);
    gStyle->SetTitleX(0.86);
    gStyle->SetTitleY(0.95);
}

double make_x_position_plot(TTree* run, int option=0){
    canvas_1 = new TCanvas();
    set_canvas_style();
    run->Draw("position_x_ >> x_position(130)");
    x_position_1 = (TH1F*)gDirectory->FindObjectAny("x_position");
    
    if(option != 0){
        title_name = "Run "+get_run_name(run).substr(3,4)+" Measured X-Position Distribution";
        x_position_1->SetTitle(title_name.c_str());
        x_position_1->GetXaxis()->SetTitle("x-position (mm)");
        x_position_1->GetXaxis()->SetTitleSize(0.05);
        x_position_1->GetXaxis()->SetLabelSize(0.05);
        x_position_1->GetYaxis()->SetTitle("count");
        x_position_1->GetYaxis()->SetTitleOffset(1.2);
        x_position_1->GetYaxis()->SetTitleSize(0.05);
        x_position_1->GetYaxis()->SetLabelSize(0.05);
        f_1 = new TF1("f_1","gaus");
        result_1 = x_position_1->Fit(f_1, "S");
        double mean_1 = result_1->Parameter(1);
        double std_dev_1 = result_1->Parameter(2);
        x_position_1->Draw();
        
        legend_1 = new TLegend(0.4,0.85,0.15,0.75);
        legend_1->SetTextSize(0.04);
        legend_1->SetBorderSize(0);
        legend_1->AddEntry(f_1, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_1,std_dev_1));
        legend_1->Draw();
        
        /*canvas_1->Update();
         TPaveText *pt = (TPaveText*)(canvas_1->GetPrimitive("title"));
         pt->SetTextSize(0.05);
         canvas_1->Modified();*/
        
        out_name = "Plots/X_Position_Plots/Measured/"+get_run_name(run)+".png";
        canvas_1->Print(out_name.c_str(),"png");
    }
    else
        canvas_1->Close();
    
    return x_position_1->GetMean();
}

void make_offset(TTree* middle_run, int sipm, int option=0){
    // time offset plot
    canvas_1 = new TCanvas();
    set_canvas_style();
    middle_run->Draw(Form("time_[0][%d]-time_[1][%d] >> Time Offset()",sipm,sipm));
    time_offset = (TH1F*)gDirectory->FindObjectAny("Time Offset");
    if(option != 0){
        time_offset->GetXaxis()->SetTitle("time (ns)");
        time_offset->GetXaxis()->SetTitleSize(0.05);
        time_offset->GetXaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetTitle("count");
        time_offset->GetYaxis()->SetTitleOffset(1.4);
        time_offset->GetYaxis()->SetTitleSize(0.05);
        time_offset->GetYaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetNdivisions(6,5,0);
        time_offset->Draw();
        out_name = "Plots/X_Position_Plots/From_Time/offset_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
    }
    else
        canvas_1->Close();
    
    // LETime offset plot
    canvas_2 = new TCanvas();
    set_canvas_style();
    middle_run->Draw(Form("LEtime_[0][%d]-LEtime_[1][%d] >> LE Time Offset()",sipm,sipm));
    LEtime_offset = (TH1F*)gDirectory->FindObjectAny("LE Time Offset");
    if(option != 0){
        LEtime_offset->GetXaxis()->SetTitle("time (ns)");
        LEtime_offset->GetXaxis()->SetTitleSize(0.05);
        LEtime_offset->GetXaxis()->SetLabelSize(0.05);
        LEtime_offset->GetYaxis()->SetTitle("count");
        LEtime_offset->GetYaxis()->SetTitleOffset(1.4);
        LEtime_offset->GetYaxis()->SetTitleSize(0.05);
        LEtime_offset->GetYaxis()->SetLabelSize(0.05);
        LEtime_offset->GetYaxis()->SetNdivisions(6,5,0);
        LEtime_offset->Draw();
        out_name = "Plots/X_Position_Plots/From_LETime/offset_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_2->Print(out_name.c_str(),"png");
    }
    else
        canvas_2->Close();
    
    // PEs offset plot
    canvas_3 = new TCanvas();
    set_canvas_style();
    middle_run->Draw(Form("PEs_[0][%d]/PEs_[1][%d] >> Channel %d PEs Offset()",sipm,sipm,sipm),Form("PEs_[0][%d]>0 && PEs_[1][%d]>0",sipm,sipm));
    PEs_offset_1 = (TH1F*)gDirectory->FindObjectAny(Form("Channel %d PEs Offset",sipm));
    if(option != 0){
        PEs_offset_1->GetXaxis()->SetTitle("Number of PEs");
        PEs_offset_1->GetXaxis()->SetTitleSize(0.05);
        PEs_offset_1->GetXaxis()->SetLabelSize(0.05);
        PEs_offset_1->GetYaxis()->SetTitle("count");
        PEs_offset_1->GetYaxis()->SetTitleOffset(1.4);
        PEs_offset_1->GetYaxis()->SetTitleSize(0.05);
        PEs_offset_1->GetYaxis()->SetLabelSize(0.05);
        PEs_offset_1->Draw();
        out_name = "Plots/X_Position_Plots/From_PEs/offset_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_3->Print(out_name.c_str(),"png");
    }
    else
        canvas_3->Close();
    
    canvas_4 = new TCanvas();
    set_canvas_style();
    middle_run->Draw(Form("PEs_[0][%d]/PEs_[1][%d] >> Channel %d PEs Offset()",sipm+1,sipm+1,sipm+1),Form("PEs_[0][%d]>0 && PEs_[1][%d]>0",sipm+1,sipm+1));
    PEs_offset_2 = (TH1F*)gDirectory->FindObjectAny(Form("Channel %d PEs Offset",sipm+1));
    if(option != 0){
        PEs_offset_2->GetXaxis()->SetTitle("Number of PEs");
        PEs_offset_2->GetXaxis()->SetTitleSize(0.05);
        PEs_offset_2->GetXaxis()->SetLabelSize(0.05);
        PEs_offset_2->GetYaxis()->SetTitle("count");
        PEs_offset_2->GetYaxis()->SetTitleOffset(1.4);
        PEs_offset_2->GetYaxis()->SetTitleSize(0.05);
        PEs_offset_2->GetYaxis()->SetLabelSize(0.05);
        PEs_offset_2->Draw();
        out_name = "Plots/X_Position_Plots/From_PEs/offset_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_4->Print(out_name.c_str(),"png");
    }
    else
        canvas_4->Close();
}

void make_offset_PEs(TTree* middle_run, int sipm, int option=0){
    canvas_1 = new TCanvas();
    gStyle->SetStatX(0.41);
    set_canvas_style();
    middle_run->Draw(Form("time_[0][%d]-time_[1][%d]:(PEs_[0][%d]+PEs_[1][%d])/2 >> Time Offset PEs(30,0,300,3,43,36)",sipm,sipm,sipm,sipm),
                     Form("time_[0][%d]-time_[1][%d]>0 && PEs_[0][%d]>0 && PEs_[1][%d]>0",sipm,sipm,sipm,sipm),"prof");
    time_offset = (TH1F*)gDirectory->FindObjectAny("Time Offset PEs");
    
    /*f_1 = new TF1("f_1","[0]*x+[1]",50,280);
    result_1 = time_offset->Fit(f_1, "RS");
    double slope = result_1->Parameter(0);
    double intercept = result_1->Parameter(1);*/
    
    if(option != 0){
        time_offset->GetXaxis()->SetTitle(Form("Average PEs in [0][%d] & [1][%d]",sipm,sipm));
        time_offset->GetXaxis()->SetTitleSize(0.05);
        time_offset->GetXaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetRangeUser(43,46);
        time_offset->GetYaxis()->SetTitle("Time Difference");
        time_offset->GetYaxis()->SetTitleOffset(1.1);
        time_offset->GetYaxis()->SetTitleSize(0.05);
        time_offset->GetYaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetNdivisions(6,5,0);
        time_offset->Draw();
        out_name = "Plots/X_Position_Plots/From_Time/offset_PEs_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
    }
    else
        canvas_1->Close();
}

void make_offset_PulseWidth(TTree* middle_run, int sipm, int option=0){
    canvas_1 = new TCanvas();
    set_canvas_style();
    middle_run->Draw(Form("time_[0][%d]-time_[1][%d]:(pulseWidth_[0][%d]+pulseWidth_[1][%d])/2 >> Time Offset Pulse Width(50,0,50)",sipm,sipm,sipm,sipm),
                     Form("time_[0][%d]-time_[1][%d]>0 && pulseWidth_[0][%d]>0 && pulseWidth_[1][%d]>0",sipm,sipm,sipm,sipm),"prof");
    time_offset = (TH1F*)gDirectory->FindObjectAny("Time Offset Pulse Width");
    
    /*f_1 = new TF1("f_1","[0]*x+[1]",10,40);
    result_1 = time_offset->Fit(f_1, "RS");
    double slope = result_1->Parameter(0);
    double intercept = result_1->Parameter(1);*/
    
    if(option != 0){
        time_offset->GetXaxis()->SetTitle(Form("Average Pulse Width in [0][%d] & [1][%d]",sipm,sipm));
        time_offset->GetXaxis()->SetTitleSize(0.05);
        time_offset->GetXaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetRangeUser(0,150);
        time_offset->GetYaxis()->SetTitle("Time Difference");
        time_offset->GetYaxis()->SetTitleOffset(1.1);
        time_offset->GetYaxis()->SetTitleSize(0.05);
        time_offset->GetYaxis()->SetLabelSize(0.05);
        time_offset->GetYaxis()->SetNdivisions(6,5,0);
        time_offset->Draw();
        out_name = "Plots/X_Position_Plots/From_Time/offset_PulseWidth_"+get_run_name(middle_run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
    }
    else
        canvas_1->Close();
}

double make_x_position_plot_from_time(TTree* run, int sipm, int option=0){
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Position from Time (Single SiPM)";
    x_position_1 = new TH1F("x_position_1",title_name.c_str(),375,-500,3250);
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Position from Time (Two Adjamcent SiPMs)";
    x_position_2 = new TH1F("x_position_2",title_name.c_str(),375,-500,3250);
    
    run->SetBranchAddress("time_",time_);
    Double_t distance_1, distance_2;
    Int_t nentries = (Int_t)run->GetEntries();
    for(Int_t j=0; j<nentries; j++){
        run->GetEntry(j);
        distance_1 = (Double_t)(time_[0][sipm]-time_[1][sipm]-time_offset->GetMean())*V_LIGHT_PEAK;
        distance_2 = (Double_t)(time_[0][sipm]+time_[0][sipm+1]-time_[1][sipm]-time_[1][sipm+1]-2*time_offset->GetMean())*.5*V_LIGHT_PEAK;
        x_position_1->Fill(1500.+distance_1/2);
        x_position_2->Fill(1500.+distance_2/2);
    }
    
    if(option != 0){
        x_position_1->GetXaxis()->SetTitle("x-position (mm)");
        x_position_1->GetXaxis()->SetTitleSize(0.05);
        x_position_1->GetXaxis()->SetLabelSize(0.05);
        x_position_1->GetXaxis()->SetLabelOffset(0.015);
        x_position_1->GetXaxis()->SetNdivisions(5,5,0);
        x_position_1->GetYaxis()->SetTitle("count");
        x_position_1->GetYaxis()->SetTitleOffset(1.2);
        x_position_1->GetYaxis()->SetTitleSize(0.05);
        x_position_1->GetYaxis()->SetLabelSize(0.05);
        x_position_1->GetYaxis()->SetRangeUser(0,2500);
        f_1 = new TF1("f_1","gaus");
        result_1 = x_position_1->Fit(f_1, "S");
        double mean_1 = result_1->Parameter(1);
        double std_dev_1 = result_1->Parameter(2);
        
        legend_1 = new TLegend(0.4,0.85,0.15,0.75);
        legend_1->SetTextSize(0.04);
        legend_1->SetBorderSize(0);
        legend_1->AddEntry(f_1, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_1,std_dev_1));
        
        canvas_1 = new TCanvas();
        set_canvas_style();
        x_position_1->Draw();
        legend_1->Draw();
        out_name = "Plots/X_Position_Plots/From_Time/"+get_run_name(run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
        
        x_position_2->GetXaxis()->SetTitle("x-position (mm)");
        x_position_2->GetXaxis()->SetTitleSize(0.05);
        x_position_2->GetXaxis()->SetLabelSize(0.05);
        x_position_2->GetXaxis()->SetLabelOffset(0.015);
        x_position_2->GetXaxis()->SetNdivisions(5,5,0);
        x_position_2->GetYaxis()->SetTitle("count");
        x_position_2->GetYaxis()->SetTitleOffset(1.2);
        x_position_2->GetYaxis()->SetTitleSize(0.05);
        x_position_2->GetYaxis()->SetLabelSize(0.05);
        x_position_2->GetYaxis()->SetRangeUser(0,2500);
        f_2 = new TF1("f_2","gaus");
        result_2 = x_position_2->Fit(f_2, "S");
        double mean_2 = result_2->Parameter(1);
        double std_dev_2 = result_2->Parameter(2);
        
        legend_2 = new TLegend(0.4,0.85,0.15,0.75);
        legend_2->SetTextSize(0.04);
        legend_2->SetBorderSize(0);
        legend_2->AddEntry(f_2, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_2,std_dev_2));

        canvas_2 = new TCanvas();
        set_canvas_style();
        x_position_2->Draw();
        legend_2->Draw();
        out_name = "Plots/X_Position_Plots/From_Time/"+get_run_name(run)+"_sipms"+to_string(sipm)+"_"+to_string(sipm+1)+".png";
        canvas_2->Print(out_name.c_str(),"png");
    }
    return x_position_2->GetMean();
}

double make_x_position_plot_from_LEtime(TTree* run, int sipm, int option=0){
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Position from LETime (Single SiPM)";
    x_position_1 = new TH1F("x_position_1",title_name.c_str(),375,-500,3250);
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Position from LETime (Two Adjamcent SiPMs)";
    x_position_2 = new TH1F("x_position_2",title_name.c_str(),375,-500,3250);
    
    run->SetBranchAddress("LEtime_",LEtime_);
    Double_t distance_1, distance_2;
    Int_t nentries = (Int_t)run->GetEntries();
    for(Int_t j=0; j<nentries; j++){
        run->GetEntry(j);
        distance_1 = (Double_t)(LEtime_[0][sipm]-LEtime_[1][sipm]-LEtime_offset->GetMean())*V_LIGHT_LE;
        distance_2 = (Double_t)(LEtime_[0][sipm]+LEtime_[0][sipm+1]-LEtime_[1][sipm]-LEtime_[1][sipm+1]-2*LEtime_offset->GetMean())*.5*V_LIGHT_LE;
        x_position_1->Fill(1500.+distance_1/2);
        x_position_2->Fill(1500.+distance_2/2);
    }
    
    if(option != 0){
        x_position_1->GetXaxis()->SetTitle("x-position (mm)");
        x_position_1->GetXaxis()->SetTitleSize(0.05);
        x_position_1->GetXaxis()->SetLabelSize(0.05);
        x_position_1->GetXaxis()->SetLabelOffset(0.015);
        x_position_1->GetXaxis()->SetNdivisions(5,5,0);
        x_position_1->GetYaxis()->SetTitle("count");
        x_position_1->GetYaxis()->SetTitleOffset(1.2);
        x_position_1->GetYaxis()->SetTitleSize(0.05);
        x_position_1->GetYaxis()->SetLabelSize(0.05);
        x_position_1->GetYaxis()->SetRangeUser(0,2500);
        TF1* f_1 = new TF1("f_1","gaus");
        result_1 = x_position_1->Fit(f_1, "S");
        double mean_1 = result_1->Parameter(1);
        double std_dev_1 = result_1->Parameter(2);
        
        legend_1 = new TLegend(0.4,0.85,0.15,0.75);
        legend_1->SetTextSize(0.04);
        legend_1->SetBorderSize(0);
        legend_1->AddEntry(f_1, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_1,std_dev_1));
        
        canvas_1 = new TCanvas();
        set_canvas_style();
        x_position_1->Draw();
        legend_1->Draw();
        out_name = "Plots/X_Position_Plots/From_LETime/"+get_run_name(run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
        
        x_position_2->GetXaxis()->SetTitle("x-position (mm)");
        x_position_2->GetXaxis()->SetTitleSize(0.05);
        x_position_2->GetXaxis()->SetLabelSize(0.05);
        x_position_2->GetXaxis()->SetLabelOffset(0.015);
        x_position_2->GetXaxis()->SetNdivisions(5,5,0);
        x_position_2->GetYaxis()->SetTitle("count");
        x_position_2->GetYaxis()->SetTitleOffset(1.2);
        x_position_2->GetYaxis()->SetTitleSize(0.05);
        x_position_2->GetYaxis()->SetLabelSize(0.05);
        x_position_2->GetYaxis()->SetRangeUser(0,2500);
        TF1* f_2 = new TF1("f_2","gaus");
        result_2 = x_position_2->Fit(f_2, "S");
        double mean_2 = result_2->Parameter(1);
        double std_dev_2 = result_2->Parameter(2);
        
        legend_2 = new TLegend(0.4,0.85,0.15,0.75);
        legend_2->SetTextSize(0.04);
        legend_2->SetBorderSize(0);
        legend_2->AddEntry(f_2, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_2,std_dev_2));
        
        canvas_2 = new TCanvas();
        set_canvas_style();
        x_position_2->Draw();
        legend_2->Draw();
        out_name = "Plots/X_Position_Plots/From_LETime/"+get_run_name(run)+"_sipms"+to_string(sipm)+"_"+to_string(sipm+1)+".png";
        canvas_2->Print(out_name.c_str(),"png");
    }
    return x_position_2->GetMean();
}

double make_x_position_plot_from_pe(TTree* run, int sipm, int option=0){
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Positions from PEs (Single SiPM)";
    x_position_1 = new TH1F("x_position_1",title_name.c_str(),125,-1000,4000);
    title_name = "Run "+get_run_name(run).substr(3,4)+" X-Position from PEs (Two Adjamcent SiPMs)";
    x_position_2 = new TH1F("x_position_2",title_name.c_str(),125,-1000,4000);
    
    int PEs_[2][16];
    run->SetBranchAddress("PEs_",PEs_);
    Double_t distance_1, distance_2;
    Int_t nentries = (Int_t)run->GetEntries();
    for(Int_t j=0; j<nentries; j++){
        run->GetEntry(j);
        
        distance_1 = (Double_t)((PEs_[0][sipm]-PEs_[1][sipm]*PEs_offset_1->GetMean())/(PEs_[0][sipm]+PEs_[1][sipm]*PEs_offset_1->GetMean()));
        distance_2 = (Double_t)(((PEs_[0][sipm]+PEs_[0][sipm+1])-(PEs_[1][sipm]*PEs_offset_1->GetMean()+PEs_[1][sipm+1]*PEs_offset_2->GetMean()))/(PEs_[0][sipm]+PEs_[0][sipm+1]+PEs_[1][sipm]*PEs_offset_1->GetMean()+PEs_[1][sipm+1]*PEs_offset_2->GetMean()));
        // unadjusted
        /*if(PEs_[0][sipm]>0 && PEs_[1][sipm]>0){
            x_position_1->Fill(1500.-3000.0*distance_1);
            if(PEs_[0][sipm+1]>0 && PEs_[1][sipm+1]>0)
                x_position_2->Fill(1500.-3000.0*distance_2);
        }*/
        // adjusted
        if(PEs_[0][sipm]>0 && PEs_[1][sipm]>0){
            x_position_1->Fill(1500.-3000.0/0.567*distance_1);
            if(PEs_[0][sipm+1]>0 && PEs_[1][sipm+1]>0)
                x_position_2->Fill(1500.-3000.0/0.567*distance_2);
        }
    }
    
    if(option != 0){
        x_position_1->GetXaxis()->SetTitle("x-position (mm)");
        x_position_1->GetXaxis()->SetTitleSize(0.05);
        x_position_1->GetXaxis()->SetLabelSize(0.05);
        x_position_1->GetXaxis()->SetLabelOffset(0.015);
        x_position_1->GetXaxis()->SetNdivisions(5,5,0);
        x_position_1->GetYaxis()->SetTitle("count");
        x_position_1->GetYaxis()->SetTitleOffset(1.2);
        x_position_1->GetYaxis()->SetTitleSize(0.05);
        x_position_1->GetYaxis()->SetLabelSize(0.05);
        x_position_1->GetYaxis()->SetRangeUser(0,5000);
        x_position_1->GetYaxis()->SetNdivisions(5,5,0);
        TF1* f_1 = new TF1("f_2","gaus");
        result_1 = x_position_1->Fit(f_1, "S");
        double mean_1 = result_1->Parameter(1);
        double std_dev_1 = result_1->Parameter(2);
        
        legend_1 = new TLegend(0.4,0.85,0.15,0.75);
        legend_1->SetTextSize(0.04);
        legend_1->SetBorderSize(0);
        legend_1->AddEntry(f_1, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_1,std_dev_1));
        
        canvas_1 = new TCanvas();
        set_canvas_style();
        x_position_1->Draw();
        legend_1->Draw();
        out_name = "Plots/X_Position_Plots/From_PEs/"+get_run_name(run)+"_sipm"+to_string(sipm)+".png";
        //out_name = "Plots/X_Position_Plots/From_PEs_Unadjusted/"+get_run_name(run)+"_sipm"+to_string(sipm)+".png";
        canvas_1->Print(out_name.c_str(),"png");
        
        x_position_2->GetXaxis()->SetTitle("x-position (mm)");
        x_position_2->GetXaxis()->SetTitleSize(0.05);
        x_position_2->GetXaxis()->SetLabelSize(0.05);
        x_position_2->GetXaxis()->SetLabelOffset(0.015);
        x_position_2->GetXaxis()->SetNdivisions(5,5,0);
        x_position_2->GetYaxis()->SetTitle("count");
        x_position_2->GetYaxis()->SetTitleOffset(1.2);
        x_position_2->GetYaxis()->SetTitleSize(0.05);
        x_position_2->GetYaxis()->SetLabelSize(0.05);
        x_position_2->GetYaxis()->SetRangeUser(0,5000);
        x_position_2->GetYaxis()->SetNdivisions(5,5,0);
        TF1* f_2 = new TF1("f_2","gaus");
        result_2 = x_position_2->Fit(f_2, "S");
        double mean_2 = result_2->Parameter(1);
        double std_dev_2 = result_2->Parameter(2);
        
        legend_2 = new TLegend(0.4,0.85,0.15,0.75);
        legend_2->SetTextSize(0.04);
        legend_2->SetBorderSize(0);
        legend_2->AddEntry(f_2, Form("Fit: #splitline{#mu = %-7.3lf}{#sigma = %-7.3lf}",mean_2,std_dev_2));
        
        canvas_2 = new TCanvas();
        set_canvas_style();
        x_position_2->Draw();
        legend_2->Draw();
        out_name = out_name = "Plots/X_Position_Plots/From_PEs/"+get_run_name(run)+"_sipms"+to_string(sipm)+"_"+to_string(sipm+1)+".png";
        //out_name = out_name = "Plots/X_Position_Plots/From_PEs_Unadjusted/"+get_run_name(run)+"_sipms"+to_string(sipm)+"_"+to_string(sipm+1)+".png";
        canvas_2->Print(out_name.c_str(),"png");
    }
    
    return x_position_2->GetMean();
}

void make_calculated_vs_measured_plot(double* measured, double* calculated, const char *title){
    graph_1 = new TGraphErrors(15,measured,calculated,nullptr,nullptr);
    graph_1->SetTitle(title);
    graph_1->SetMarkerStyle(8);
    graph_1->SetMarkerColor(kBlack);
    graph_1->SetLineStyle(7);
    graph_1->SetLineColor(kBlack);
    
    graph_1->GetXaxis()->SetTitle("Beam Position (mm)");
    graph_1->GetXaxis()->SetTitleOffset(1.2);
    graph_1->GetXaxis()->SetTitleSize(0.05);
    graph_1->GetXaxis()->SetLabelSize(0.05);
    graph_1->GetXaxis()->SetLabelOffset(0.015);
    graph_1->GetXaxis()->SetNdivisions(5,5,0);
    graph_1->GetYaxis()->SetTitle("Calculated (mm)");
    graph_1->GetYaxis()->SetTitleOffset(1.2);
    graph_1->GetYaxis()->SetTitleSize(0.05);
    graph_1->GetYaxis()->SetLabelSize(0.05);
    graph_1->GetYaxis()->SetNdivisions(5,5,0);
    
    f_1 = new TF1("f_1","[0]*x+[1]",500,2000);
    result_1 = graph_1->Fit(f_1, "RS");
    double slope = result_1->Parameter(0);
    double intercept = result_1->Parameter(1);
    
    legend_1 = new TLegend(0.4,0.85,0.15,0.75);
    legend_1->SetTextSize(0.04);
    legend_1->SetBorderSize(0);
    legend_1->AddEntry(f_1, Form("Fit: #splitline{slope = %-7.3lf}{y-intercept = %-7.3lf}",slope,intercept));
    
    canvas_1 = new TCanvas();
    set_canvas_style();
    graph_1->Draw();
    legend_1->Draw();
    
    string s(title);
    out_name = "Plots/X_Position_Plots/Calculated_vs_Measured/"+s+".png";
    canvas_1->Print(out_name.c_str(),"png");
}
