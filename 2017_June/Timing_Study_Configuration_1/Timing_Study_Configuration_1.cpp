# include <stdio.h>
# include <string>
# include <vector>

using namespace std;

const int NUM_RUNS = 15;

void set_canvas_style();
TLegend* set_graph_style(TGraph* graph, const char* title, const char* x_title, const char* y_title);
TLegend* set_histogram_style(TH1F* histo, const char* title, const char* x_title, const char* y_title, int min, int max);
TTree* get_tree_from_run(int run_num);
void resolution_single_counter(int channel);
void resolution_single_fiber(int channel);
void time_LEtime_difference(int channel);
void light_speed_peak_time(int channel);
void light_speed_LEtime(int channel);

void Timing_Study_Configuration_1(){
    /*
    for(int i = 4; i <= 7; i++) {
        light_speed_peak_time(i);
        light_speed_LEtime(i);
    }
     */
    //resolution_single_counter(6);
    //resolution_single_fiber(6);
    //resolution_single_fiber(7);
    
    time_LEtime_difference(7);
}

void set_canvas_style() {
    gPad->SetMargin(0.15,0.05,0.15,0.15);
    gStyle->SetTitleAlign(35);
    gStyle->SetTitleX(0.895);
    gStyle->SetTitleY(0.95);
    gStyle->SetOptStat("0");
    /*
     gStyle->SetStatW(0.25);
     gStyle->SetStatH(0.2);
     gStyle->SetStatY(0.9);
     */
}

TLegend* set_graph_style(TGraph* graph, const char* title, const char* x_title, const char* y_title) {
    graph->SetTitle(title);
    graph->SetMarkerStyle(8);
    graph->SetMarkerColor(kBlack);
    graph->SetLineStyle(7);
    graph->SetLineColor(kBlack);
    graph->GetXaxis()->SetTitle(x_title);
    graph->GetXaxis()->SetTitleOffset(1.2);
    graph->GetXaxis()->SetTitleSize(0.05);
    graph->GetXaxis()->SetLabelSize(0.05);
    graph->GetXaxis()->SetLabelOffset(0.015);
    graph->GetXaxis()->SetLimits(-10,20);
    graph->GetYaxis()->SetTitle(y_title);
    graph->GetYaxis()->SetTitleOffset(1.3);
    graph->GetYaxis()->SetTitleSize(0.05);
    graph->GetYaxis()->SetLabelSize(0.05);
    
    TF1* f = new TF1("f","[0]*x+[1]",-8,12.5);
    TFitResultPtr result = graph->Fit(f, "ERS");
    double slope = result->Parameter(0);
    double slope_error = result->ParError(0);
    //double intercept = result->Parameter(1);
    
    TLegend* legend = new TLegend(0.45,0.8,0.2,0.7,Form("Slope of linear fit: %-7.3lf #pm %-7.3lf mm/ns",slope,slope_error));
    legend->SetTextSize(0.04);
    legend->SetBorderSize(0);
    //legend->AddEntry(f, Form("Fit: #splitline{slope = %-7.3lf}{y-intercept = %-7.3lf}",slope,intercept));
    
    return legend;
}

TLegend* set_histogram_style(TH1F* histo, const char* title, const char* x_title, const char* y_title, int min=0, int max=0) {
    histo->SetTitle(title);
    histo->GetXaxis()->SetTitle(x_title);
    histo->GetXaxis()->SetTitleOffset(1.2);
    histo->GetXaxis()->SetTitleSize(0.05);
    histo->GetXaxis()->SetLabelSize(0.05);
    histo->GetXaxis()->SetLabelOffset(0.015);
    histo->GetYaxis()->SetTitle(y_title);
    histo->GetYaxis()->SetTitleOffset(1.3);
    histo->GetYaxis()->SetTitleSize(0.05);
    histo->GetYaxis()->SetLabelSize(0.05);
    histo->GetYaxis()->SetRangeUser(0,750);
    
    if(min == max) {
        histo->GetXaxis()->SetTitleOffset(1);
        histo->GetYaxis()->SetTitleOffset(0.9);
        return NULL;
    }
        
    TF1* gau = new TF1("gau","gaus",min,max);
    gau->SetLineColor(kGreen+2);
    histo->Fit(gau,"QR+");
    gau = histo->GetFunction("gau");
    
    TLegend* legend = new TLegend(0.925,0.8,0.5,0.7);
    legend->AddEntry((TObject*)0, Form("Gaussian mean: %-1.2f #pm %-1.2f ns",gau->GetParameter(1),gau->GetParError(1)), "");
    legend->AddEntry((TObject*)0, Form("Gaussian sigma: %-1.2f #pm %-1.2f ns",gau->GetParameter(2),gau->GetParError(2)), "");
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    
    return legend;
}

TTree* get_tree_from_run(int run_num) {
    gSystem->cd("../");
    TFile* file;
    if(run_num < 1000)
        file = new TFile(Form("Run_0%dmerged.root", run_num),"READ");
    else
        file = new TFile(Form("Run_%dmerged.root", run_num),"READ");
    gSystem->cd("Timing_Study_Configuration_1");
    if(file == NULL)
        return NULL;
    if(file->GetListOfKeys()->Contains(Form("run%d", run_num))) {
        TTree* run = (TTree*)file->Get(Form("run%d", run_num));
        if(run->GetEntries() < 3000)
            return NULL;
        return run;
    }
    return NULL;
}

void resolution_single_counter(int channel) {
    TTree* run = get_tree_from_run(1007);
    
    int min = -10, max = 10;
    
    TCanvas* canvas_1 = new TCanvas();
    set_canvas_style();
    run->Draw(Form("time_[0][%d]-time_[0][%d] >> resolution_peak_time(350,-15,20)",channel,channel+1));
    //run->Draw(Form("(time_[0][%d]-time_[0][%d])+(time_[1][%d]-time_[1][%d]) >> resolution_peak_time(350,-15,20)",channel,channel+1,channel,channel+1));
    TH1F* resolution_peak_time = (TH1F*)gDirectory->FindObjectAny("resolution_peak_time");
    TLegend* legend_1 = set_histogram_style(resolution_peak_time,Form("Peak Time Single Counter End Using Channel %d",channel),"Delta t (ns)","Events / 0.1 ns",min,max);
    legend_1->Draw();
    canvas_1->Print(Form("Resolution/single_counter_peak_time_channel_%d.png",channel),"png");
    
    TCanvas* canvas_2 = new TCanvas();
    set_canvas_style();
    run->Draw(Form("LEtime_[0][%d]-LEtime_[0][%d] >> resolution_LE_time(350,-15,20)",channel,channel+1));
    //run->Draw(Form("(LEtime_[0][%d]-LEtime_[0][%d])+(LEtime_[1][%d]-LEtime_[1][%d]) >> resolution_LE_time(350,-15,20)",channel,channel+1,channel,channel+1));
    TH1F* resolution_LE_time = (TH1F*)gDirectory->FindObjectAny("resolution_LE_time");
    TLegend* legend_2 = set_histogram_style(resolution_LE_time,Form("LE Time Single Counter End Using Channel %d",channel),"Delta t (ns)","Events / 0.1 ns",min,max);
    legend_2->Draw();
    canvas_2->Print(Form("Resolution/single_counter_LE_time_channel_%d.png",channel),"png");
}

void resolution_single_fiber(int channel) {
    TTree* run = get_tree_from_run(1007);
    
    int min = 30, max = 60;
    
    TCanvas* canvas_1 = new TCanvas();
    set_canvas_style();
    run->Draw(Form("time_[0][%d]-time_[1][%d] >> resolution_peak_time(300,30,60)",channel,channel));
    TH1F* resolution_peak_time = (TH1F*)gDirectory->FindObjectAny("resolution_peak_time");
    TLegend* legend_1 = set_histogram_style(resolution_peak_time,Form("Peak Time Single Fiber End Using Channel %d",channel),"Delta t (ns)","Events / 0.1 ns",min,max);
    legend_1->Draw();
    canvas_1->Print(Form("Resolution/single_fiber_peak_time_channel_%d.png",channel),"png");
    
    TCanvas* canvas_2 = new TCanvas();
    set_canvas_style();
    run->Draw(Form("LEtime_[0][%d]-LEtime_[1][%d] >> resolution_LE_time(300,30,60)",channel,channel));
    TH1F* resolution_LE_time = (TH1F*)gDirectory->FindObjectAny("resolution_LE_time");
    TLegend* legend_2 = set_histogram_style(resolution_LE_time,Form("LE Time Single Fiber End Using Channel %d",channel),"Delta t (ns)","Events / 0.1 ns",min,max);
    legend_2->Draw();
    canvas_2->Print(Form("Resolution/single_fiber_LE_time_channel_%d.png",channel),"png");
}

void time_LEtime_difference(int channel) {
    TCanvas* canvas_1;
    TCanvas* canvas_2;
    TTree* run;
    vector<TH1F*> hists_time;
    vector<TH1F*> hists_pWidth;
    
    canvas_1 = new TCanvas();
    for(int i = 1009; i <=1018; i++) {
        run = get_tree_from_run(i);
        run->Draw(Form("time_[0][%d]-LEtime_[0][%d]:pulseWidth_[0][%d] >> time_difference_vs_pWidth(50,0,50,60,0,60)",channel,channel,channel),"","");
        run->Draw(Form("time_[0][%d]-LEtime_[0][%d]:position_x_ >> time_difference_vs_x(1200,-100,1100,60,0,60)",channel,channel),"","");
        run->Draw(Form("pulseWidth_[0][%d]:position_x_ >> pWidth_vs_x(1200,-100,1100,50,0,50)",channel),"","");
        canvas_1->Close();
        
        canvas_1 = new TCanvas();
        TH1F* time_difference_vs_pWidth = (TH1F*)gDirectory->FindObjectAny("time_difference_vs_pWidth");
        set_histogram_style(time_difference_vs_pWidth,Form("Peak & LE Difference vs. Pulse Width Using Channel %d Run %d",channel,i),"pulse width (?)","peak and LE time difference (ns)");
        time_difference_vs_pWidth->Draw();
        canvas_1->Print(Form("Time_PulseWidth_XPosition/peak_LE_time_difference_vs_pulseWidth_%d_run_%d.png",channel,i),"png");
        
        TH1F* time_difference_vs_x = (TH1F*)gDirectory->FindObjectAny("time_difference_vs_x");
        TH1F* pWidth_vs_x = (TH1F*)gDirectory->FindObjectAny("pWidth_vs_x");
        
        hists_time.push_back(time_difference_vs_x);
        hists_pWidth.push_back(pWidth_vs_x);
        canvas_1->Close();
    }
    
    set_histogram_style(hists_time.at(0),Form("Peak & LE Time Difference vs. X-Position Using Channel %d",channel),"x-position (mm)","peak and LE time difference (ns)");
    set_histogram_style(hists_pWidth.at(0),Form("Pulse Width vs. X-Position Using Channel %d",channel),"x-position (mm)","pulse width (?)");
    
    canvas_1 = new TCanvas();
    for(int i = 0; i < hists_time.size(); i++)
        hists_time.at(i)->Draw("SAME");
    
    canvas_2 = new TCanvas();
    for(int i = 0; i < hists_pWidth.size(); i++)
        hists_pWidth.at(i)->Draw("SAME");
    
    canvas_1->Print(Form("Time_PulseWidth_XPosition/peak_LE_time_difference_vs_x_channel_%d.png",channel),"png");
    canvas_2->Print(Form("Time_PulseWidth_XPosition/pulseWidth_vs_x_channel_%d.png",channel),"png");
}

void light_speed_peak_time(int channel) {
    TCanvas* canvas_1 = new TCanvas();
    TTree* run;
    double avg_path_diff[NUM_RUNS];
    double avg_time_diff[NUM_RUNS];
    
    // top counter
    if(channel%4 >= 2) {
        run = get_tree_from_run(1007);
        run->Draw(Form("time_[1][%d]-time_[0][%d] >> time_difference()",channel,channel));
        TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
        double off_set = (double)time_difference->GetMean();
        canvas_1->Close();
        
        int j = 0;
        for(int i=1002; i<=1018; i++) {
            if(i==1003 || i==1004) continue;
            run = get_tree_from_run(i);
            canvas_1 = new TCanvas();
            
            run->Draw("position_x_ >> x_position()");
            TH1F* x_position = (TH1F*)gDirectory->FindObjectAny("x_position");
            double x = (double)x_position->GetMean();
            avg_path_diff[j]=3000.0-2*x;
            
            run->Draw(Form("time_[1][%d]-time_[0][%d]-%lf >> time_difference()",channel,channel,off_set));
            TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
            double dt = (double)time_difference->GetMean();
            avg_time_diff[j] = dt;
            
            //cout << x << " " << 3000-2*x << " " << dt << " " << (3000-2*x)/dt << endl;
            canvas_1->Close();
            j++;
        }
    }
    // bottom counter
    else {
        run = get_tree_from_run(1030);
        run->Draw(Form("time_[1][%d]-time_[0][%d] >> time_difference()",channel,channel));
        TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
        double off_set = (double)time_difference->GetMean();
        canvas_1->Close();
        
        int j = 0;
        for(int i=1019; i<=1033; i++){
            run = get_tree_from_run(i);
            canvas_1 = new TCanvas();
            
            run->Draw("position_x_ >> x_position()");
            TH1F* x_position = (TH1F*)gDirectory->FindObjectAny("x_position");
            double x = (double)x_position->GetMean();
            avg_path_diff[j]=3000.0-2*x;
            
            run->Draw(Form("time_[1][%d]-time_[0][%d]-%lf >> time_difference()",channel,channel,off_set));
            TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
            double dt = (double)time_difference->GetMean();
            avg_time_diff[j] = dt;
            
            //cout << x << " " << 3000-2*x << " " << dt << " " << (3000-2*x)/dt << endl;
            canvas_1->Close();
            j++;
        }
    }
    
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    TGraph* graph_1 = new TGraph(NUM_RUNS,avg_time_diff,avg_path_diff);
    TLegend* legend_1 = set_graph_style(graph_1, Form("Path Difference vs. Peak Time Difference Using Channel %d",channel), "Measured peak time difference (ns)", "Average path difference (mm)");
    graph_1->Draw();
    legend_1->Draw();
    
    canvas_1->Print(Form("Light_Speed/peak_time_channel_%d.png",channel),"png");
}

void light_speed_LEtime(int channel){
    TCanvas* canvas_1 = new TCanvas();
    TTree* run;
    double avg_path_diff[NUM_RUNS];
    double avg_time_diff[NUM_RUNS];
    
    // top counter
    if(channel%4 >= 2) {
        run = get_tree_from_run(1007);
        run->Draw(Form("LEtime_[1][%d]-LEtime_[0][%d] >> time_difference()",channel,channel));
        TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
        double off_set = (double)time_difference->GetMean();
        canvas_1->Close();
        
        int j = 0;
        for(int i=1002; i<=1018; i++){
            if(i==1003 || i==1004) continue;
            run = get_tree_from_run(i);
            canvas_1 = new TCanvas();
            
            run->Draw("position_x_ >> x_position()");
            TH1F* x_position = (TH1F*)gDirectory->FindObjectAny("x_position");
            double x = (double)x_position->GetMean();
            avg_path_diff[j]=3000.0-2*x;
            
            run->Draw(Form("LEtime_[1][%d]-LEtime_[0][%d]-%lf >> time_difference()",channel,channel,off_set));
            TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
            double dt = (double)time_difference->GetMean();
            avg_time_diff[j] = dt;
            
            //cout << x << " " << 3000-2*x << " " << dt << " " << (3000-2*x)/dt << endl;
            canvas_1->Close();
            j++;
        }
    }
    // bottom counter
    else {
        run = get_tree_from_run(1030);
        run->Draw(Form("LEtime_[1][%d]-LEtime_[0][%d] >> time_difference()",channel,channel));
        TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
        double off_set = (double)time_difference->GetMean();
        canvas_1->Close();
        
        int j = 0;
        for(int i=1019; i<=1033; i++){
            run = get_tree_from_run(i);
            canvas_1 = new TCanvas();
            
            run->Draw("position_x_ >> x_position()");
            TH1F* x_position = (TH1F*)gDirectory->FindObjectAny("x_position");
            double x = (double)x_position->GetMean();
            avg_path_diff[j]=3000.0-2*x;
            
            run->Draw(Form("LEtime_[1][%d]-LEtime_[0][%d]-%lf >> time_difference()",channel,channel,off_set));
            TH1F* time_difference = (TH1F*)gDirectory->FindObjectAny("time_difference");
            double dt = (double)time_difference->GetMean();
            avg_time_diff[j] = dt;
            
            //cout << x << " " << 3000-2*x << " " << dt << " " << (3000-2*x)/dt << endl;
            canvas_1->Close();
            j++;
        }
    }
    
    canvas_1 = new TCanvas();
    set_canvas_style();
    
    TGraph* graph_1 = new TGraph(NUM_RUNS,avg_time_diff,avg_path_diff);
    TLegend* legend_1 = set_graph_style(graph_1, Form("Path Difference vs. LE Time Difference Using Channel %d",channel), "Measured LE time difference (ns)", "Average path difference (mm)");
    graph_1->Draw();
    legend_1->Draw();
    
    canvas_1->Print(Form("Light_Speed/LEtime_channel_%d.png",channel),"png");
}