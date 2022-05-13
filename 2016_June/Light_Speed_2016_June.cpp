# include <stdio.h>
# include <string>

using namespace std;

const int CHANNEL = 11, NUM_RUNS = 11;
double average_distance_difference[NUM_RUNS];
double average_time_difference[NUM_RUNS];
TTree* run;
TFile* file_run;
TCanvas* canvas_1;
TH1F* x_position;
TH1F* time_difference;

void Light_Speed_2016_June(){
    file_run = new TFile("Run_0812merged.root");
    file_run->GetObject("run812",run);
    canvas_1 = new TCanvas();
    run->Draw(Form("time_[0][%d]-time_[1][%d] >> time_difference()",CHANNEL,CHANNEL));
    time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
    double off_set = (double)time_difference->GetMean();
    canvas_1->Close();
    
    int j = 0;
    for(int i=796; i<=818; i++){
        if(i==797||i==798||i==801||i==802||i==805||i==806||i==809||i==810||i==811||i==814||i==815||i==818) continue;
        file_run = new TFile(Form("Run_0%dmerged.root",i));
        file_run->GetObject(Form("run%d",i),run);
        
        canvas_1 = new TCanvas();
        
        run->Draw("position_x_ >> x_position()");
        x_position = (TH1F*)gDirectory->FindObjectAny("x_position");
        double x = (double)x_position->GetMean();
        average_distance_difference[j]=3000.0-2*x;
        
        run->Draw(Form("time_[0][%d]-time_[1][%d]-%lf >> time_difference()",CHANNEL,CHANNEL,off_set));
        time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
        double dt = (double)time_difference->GetMean();
        average_time_difference[j] = dt;
        
        cout << x << " " << 3000-2*x << " " << dt << " " << (3000-2*x)/dt << endl;

        file_run->Close();
        canvas_1->Close();
        j++;
    }
    
    canvas_1 = new TCanvas();
    gPad->SetMargin(0.125,0.1,0.125,0.125);
    gStyle->SetTitleSize(0.06,"t");
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.2);
    gStyle->SetStatY(0.9);
    
    TGraph* graph_1 = new TGraph(NUM_RUNS,average_time_difference,average_distance_difference);
    graph_1->SetTitle("Distance Difference vs. Peak Time Difference");
    graph_1->SetMarkerStyle(8);
    graph_1->SetMarkerColor(kBlack);
    graph_1->SetLineStyle(7);
    graph_1->SetLineColor(kBlack);
    
    graph_1->GetXaxis()->SetTitle("Average Peak Time Difference (ns)");
    graph_1->GetXaxis()->SetTitleOffset(1.2);
    graph_1->GetXaxis()->SetTitleSize(0.05);
    graph_1->GetXaxis()->SetLabelSize(0.05);
    graph_1->GetXaxis()->SetLabelOffset(0.015);
    //graph_1->GetXaxis()->SetNdivisions(5,5,0);
    graph_1->GetYaxis()->SetTitle("Average Distance Difference (mm)");
    graph_1->GetYaxis()->SetTitleOffset(1.2);
    graph_1->GetYaxis()->SetTitleSize(0.05);
    graph_1->GetYaxis()->SetLabelSize(0.05);
    //graph_1->GetYaxis()->SetNdivisions(5,5,0);
    
    TF1* f_1 = new TF1("f_1","[0]*x+[1]",-7.5,15);
    TFitResultPtr result_1 = graph_1->Fit(f_1, "RS");
    double slope = result_1->Parameter(0);
    double intercept = result_1->Parameter(1);
    
    TLegend* legend_1 = new TLegend(0.4,0.85,0.15,0.75);
    legend_1->SetTextSize(0.04);
    legend_1->SetBorderSize(0);
    legend_1->AddEntry(f_1, Form("Fit: #splitline{slope = %-7.3lf}{y-intercept = %-7.3lf}",slope,intercept));
    
    graph_1->Draw();
    legend_1->Draw();
    
    canvas_1->Print(Form("Plots/Light_Speed/channel_%d.png",CHANNEL),"png");
}
