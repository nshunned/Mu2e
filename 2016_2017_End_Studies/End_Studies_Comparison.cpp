# include <string>

using namespace std;

void set_canvas_style() {
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
}

void End_Studies_Comparison(){
    TFile* file_2016 = new TFile("2016_sum_scaled_histograms.root");
    TProfile* default_2016;
    TProfile* white_paint_2016;
    file_2016->GetObject("Default",default_2016);
    file_2016->GetObject("White_Paint",white_paint_2016);
    
    TFile* file_2017 = new TFile("2017_sum_scaled_histograms.root");
    TProfile* default_2017;
    TProfile* white_paint_2017;
    file_2017->GetObject("Default",default_2017);
    file_2017->GetObject("White_Paint",white_paint_2017);
    
    // default
    TCanvas* canvas_1 = new TCanvas();
    set_canvas_style();
    
    default_2016->SetLineColor(kBlue);
    default_2016->SetTitle("Default Counters Summary");
    default_2016->GetYaxis()->SetRangeUser(0,1);
    
    TLegend* legend_1 = new TLegend(0.88, 0.28, 0.57, 0.155, "");
    legend_1->SetTextSize(0.05);
    legend_1->AddEntry(default_2016, "February 2016");
    legend_1->AddEntry(default_2017, "June 2017");
    
    default_2016->Draw();
    default_2017->Draw("SAME");
    legend_1->Draw();
    
    canvas_1->Print("default.png","png");
    
    // white_paint
    TCanvas* canvas_2 = new TCanvas();
    set_canvas_style();
    
    white_paint_2017->SetLineColor(kRed);
    white_paint_2016->SetTitle("White Paint Counters Summary");
    white_paint_2016->GetXaxis()->SetRangeUser(-3,10);
    white_paint_2016->GetXaxis()->SetTitle("adjusted x-position (mm)");
    white_paint_2016->GetXaxis()->SetTitleSize(0.05);
    white_paint_2016->GetXaxis()->SetTitleOffset(1.2);
    white_paint_2016->GetXaxis()->SetLabelSize(0.06);
    white_paint_2016->GetYaxis()->SetRangeUser(0,1);
    white_paint_2016->GetYaxis()->SetTitle("normalized response");
    white_paint_2016->GetYaxis()->SetTitleSize(0.05);
    white_paint_2016->GetYaxis()->SetLabelSize(0.06);
    white_paint_2016->GetYaxis()->SetTitleOffset(1.2);
    
    TLegend* legend_2 = new TLegend(0.88, 0.28, 0.57, 0.155, "");
    legend_2->SetTextSize(0.05);
    legend_2->AddEntry(white_paint_2016, "February 2016");
    legend_2->AddEntry(white_paint_2017, "June 2017");
    
    white_paint_2016->Draw();
    white_paint_2017->Draw("SAME");
    legend_2->Draw();
    
    canvas_2->Print("white_paint_paint.png","png");
}
