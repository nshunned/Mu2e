/*
 y = 25mm
 [0][4] --- [1][8]
 [0][5] --- [1][9]
 y = 75mm
 [0][6] --- [1][10]
 [0][7] --- [1][11]
 */

# include <string>

using namespace std;

const int NUM_RUNS = 18;

void set_canvas_style(){
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
}

void set_graph_style(TGraphErrors *graph){
    graph->SetMarkerStyle(8);
    graph->SetMarkerColor(kBlack);
    graph->SetLineStyle(7);
    graph->SetLineColor(kBlack);
}

void set_graph_style_2(TGraphErrors *graph) {
    graph->SetMarkerStyle(29);
    graph->SetMarkerSize(2.5);
}

void set_multigraph_style(TMultiGraph *graph, const char* title, const char* y_title) {
    // graph->SetTitle(title);
    graph->SetTitle("");
    graph->GetXaxis()->SetTitle("Bias Voltage [V]");
    graph->GetXaxis()->SetTitleOffset(1.2);
    graph->GetXaxis()->SetTitleSize(0.05);
    graph->GetXaxis()->SetLabelSize(0.05);
    graph->GetXaxis()->SetLabelOffset(0.015);
    graph->GetYaxis()->SetTitle(y_title);
    graph->GetYaxis()->SetTitleOffset(1.2);
    graph->GetYaxis()->SetTitleSize(0.05);
    graph->GetYaxis()->SetLabelSize(0.05);
}

void make_sum_plots(){
    Double_t bias_array[NUM_RUNS];
    Double_t fwhm_array[NUM_RUNS];
    Double_t mpv_array[NUM_RUNS];
    Double_t fwhm_div_mpv_array[NUM_RUNS];
    Double_t noise_array[NUM_RUNS];
    Double_t cross_talk_array[NUM_RUNS];
    
    for(int i=0; i < NUM_RUNS; i++){
        bias_array[i] = 52.1 + 0.5*i;
    }
    
    TGraphErrors *fwhm_graph;
    TGraphErrors *mpv_graph;
    TGraphErrors *fwhm_div_mpv_graph;
    TGraphErrors *noise_graph;
    TGraphErrors *cross_talk_graph;
    
    TFile *file_1;
    TFile *file_2;
    TFile *file_3;
    TFile *file_4;
    TFile *file_5;
    
    for(int i = 4; i <= 7; i++){
        file_1 = new TFile(Form("Bias_Correlation_Plots/[0][%i]_fwhm_vs_bias.root", i),"READ");
        file_2 = new TFile(Form("Bias_Correlation_Plots/[0][%i]_mpv_vs_bias.root", i),"READ");
        file_3 = new TFile(Form("Bias_Correlation_Plots/[0][%i]_fwhm_to_mpv_vs_bias.root", i),"READ");
        file_4 = new TFile(Form("Bias_Correlation_Plots/[0][%i]_noise_vs_bias.root", i),"READ");
        file_5 = new TFile(Form("Bias_Correlation_Plots/[0][%i]_cross_talk_vs_bias.root", i),"READ");
        
        fwhm_graph = (TGraphErrors*)file_1->Get("bias");
        mpv_graph = (TGraphErrors*)file_2->Get("bias");
        fwhm_div_mpv_graph = (TGraphErrors*)file_3->Get("bias");
        noise_graph = (TGraphErrors*)file_4->Get("bias");
        cross_talk_graph = (TGraphErrors*)file_5->Get("bias");
        
        for(int j=0; j < NUM_RUNS; j++){
            fwhm_array[j] +=  (fwhm_graph->GetY())[j];
            mpv_array[j] += (mpv_graph->GetY())[j];
            fwhm_div_mpv_array[j] += (fwhm_div_mpv_graph->GetY())[j];
            noise_array[j] += (noise_graph->GetY())[j];
            cross_talk_array[j] += (cross_talk_graph->GetY())[j];
        }
        
        file_1->Close();
        file_2->Close();
        file_3->Close();
        file_4->Close();
        file_5->Close();
    }
    
    for(int j=0; j < NUM_RUNS; j++){
        fwhm_array[j] /=  4;
        mpv_array[j] /= 4;
        fwhm_div_mpv_array[j] /= 4;
        noise_array[j] /= 4;
        cross_talk_array[j] /= 4;
    }
    
    fwhm_graph = new TGraphErrors(NUM_RUNS, bias_array, fwhm_array, nullptr, nullptr);
    mpv_graph = new TGraphErrors(NUM_RUNS, bias_array, mpv_array, nullptr, nullptr);
    fwhm_div_mpv_graph = new TGraphErrors(NUM_RUNS, bias_array, fwhm_div_mpv_array, nullptr, nullptr);
    noise_graph = new TGraphErrors(NUM_RUNS, bias_array, noise_array, nullptr, nullptr);
    cross_talk_graph = new TGraphErrors(NUM_RUNS, bias_array, cross_talk_array, nullptr, nullptr);
    
    // removes unusable data points
    fwhm_graph->RemovePoint(0);
    mpv_graph->RemovePoint(0);
    fwhm_div_mpv_graph->RemovePoint(0);
    noise_graph->RemovePoint(0);
    cross_talk_graph->RemovePoint(0);
    
    fwhm_graph->RemovePoint(0);
    mpv_graph->RemovePoint(0);
    fwhm_div_mpv_graph->RemovePoint(0);
    noise_graph->RemovePoint(0);
    cross_talk_graph->RemovePoint(0);
    
    fwhm_graph->RemovePoint(6);
    mpv_graph->RemovePoint(6);
    fwhm_div_mpv_graph->RemovePoint(6);
    noise_graph->RemovePoint(6);
    cross_talk_graph->RemovePoint(6);
    
    fwhm_graph->RemovePoint(13);
    mpv_graph->RemovePoint(13);
    fwhm_div_mpv_graph->RemovePoint(13);
    noise_graph->RemovePoint(13);
    cross_talk_graph->RemovePoint(13);
    
    fwhm_graph->RemovePoint(13);
    mpv_graph->RemovePoint(13);
    fwhm_div_mpv_graph->RemovePoint(13);
    noise_graph->RemovePoint(13);
    cross_talk_graph->RemovePoint(13);
    
    set_graph_style(fwhm_graph);
    set_graph_style(mpv_graph);
    set_graph_style(fwhm_div_mpv_graph);
    set_graph_style(noise_graph);
    set_graph_style(cross_talk_graph);
    
    TGraphErrors *fwhm_graph_2 = new TGraphErrors(1, &bias_array[6], &fwhm_array[6], nullptr, nullptr);
    TGraphErrors *mpv_graph_2 = new TGraphErrors(1, &bias_array[6], &mpv_array[6], nullptr, nullptr);
    TGraphErrors *fwhm_div_mpv_graph_2 = new TGraphErrors(1, &bias_array[6], &fwhm_div_mpv_array[6], nullptr, nullptr);
    TGraphErrors *noise_graph_2 = new TGraphErrors(1, &bias_array[6], &noise_array[6], nullptr, nullptr);
    TGraphErrors *cross_talk_graph_2 = new TGraphErrors(1, &bias_array[6], &cross_talk_array[6], nullptr, nullptr);
    
    set_graph_style_2(fwhm_graph_2);
    set_graph_style_2(mpv_graph_2);
    set_graph_style_2(fwhm_div_mpv_graph_2);
    set_graph_style_2(noise_graph_2);
    set_graph_style_2(cross_talk_graph_2);
    
    TCanvas *canvas_1 = new TCanvas();
    set_canvas_style();
    TMultiGraph *mg_1 = new TMultiGraph();
    mg_1->Add(fwhm_graph,"LP");
    mg_1->Add(fwhm_graph_2,"LP");
    mg_1->Draw("A");
    set_multigraph_style(mg_1, "FWHM vs. Bias", "Full-Width at Half-Max (PE)");
    mg_1->GetYaxis()->SetRangeUser(0,75);
    gPad->Update();
    canvas_1->Print("Bias_Correlation_Plots_Merged/fwhm_vs_bias_merged.png","png");
    
    TCanvas *canvas_2 = new TCanvas();
    set_canvas_style();
    TMultiGraph *mg_2 = new TMultiGraph();
    mg_2->Add(mpv_graph,"LP");
    mg_2->Add(mpv_graph_2,"LP");
    mg_2->Draw("A");
    set_multigraph_style(mg_2, "MPV vs. Bias", "Most Probable Value (PE)");
    mg_2->GetYaxis()->SetRangeUser(0,250);
    gPad->Update();
    canvas_2->Print("Bias_Correlation_Plots_Merged/mpv_vs_bias_merged.png","png");
    
    TCanvas *canvas_3 = new TCanvas();
    set_canvas_style();
    TMultiGraph *mg_3 = new TMultiGraph();
    mg_3->Add(fwhm_div_mpv_graph,"LP");
    mg_3->Add(fwhm_div_mpv_graph_2,"LP");
    mg_3->Draw("A");
    set_multigraph_style(mg_3, "FWHM/MPV vs. Bias", "FWHM/MPV");
    mg_3->GetYaxis()->SetRangeUser(0.3,0.5);
    gPad->Update();
    canvas_3->Print("Bias_Correlation_Plots_Merged/fwhm_to_mpv_vs_bias_merged.png","png");
    
    TCanvas *canvas_4 = new TCanvas();
    set_canvas_style();
    TMultiGraph *mg_4 = new TMultiGraph();
    mg_4->Add(noise_graph,"LP");
    mg_4->Add(noise_graph_2,"LP");
    mg_4->Draw("A");
    set_multigraph_style(mg_4, "Noise vs. Bias", "Noise Rate (MHz)");
    mg_4->GetYaxis()->SetRangeUser(0,0.5);
    gPad->Update();
    canvas_4->Print("Bias_Correlation_Plots_Merged/noise_vs_bias_merged.png","png");
    
    TCanvas *canvas_5 = new TCanvas();
    set_canvas_style();
    TMultiGraph *mg_5 = new TMultiGraph();
    mg_5->Add(cross_talk_graph,"LP");
    mg_5->Add(cross_talk_graph_2,"LP");
    mg_5->Draw("A");
    set_multigraph_style(mg_5, "Cross Talk Probability vs. Bias", "Cross Talk Probability (%)");
    mg_5->GetYaxis()->SetRangeUser(0,35);
    gPad->Update();
    canvas_5->Print("Bias_Correlation_Plots_Merged/cross_talk_vs_bias_merged.png","png");
}

void Bias_Study_Configuration_2_Merge(){
    make_sum_plots();
}
