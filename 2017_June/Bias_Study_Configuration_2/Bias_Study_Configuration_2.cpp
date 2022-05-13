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
const double X_MIN = 0, X_MAX = 400;
TF1* pe_distribution[NUM_RUNS];
double bias[NUM_RUNS];
double mpv[NUM_RUNS];
double fwhm[NUM_RUNS];
double fwhm_div_mpv[NUM_RUNS];

double noise_ch4[NUM_RUNS] = {0.02, 0.03, 0.09, 0.11, 0.14, 0.14, 0.17, 0.19, 0.17, 0.23, 0.25, 0.28, 0.30, 0.32, 0.34, 0.38, 0.42, 0.48};
double noise_ch5[NUM_RUNS] = {0.00, 0.02, 0.07, 0.10, 0.12, 0.14, 0.16, 0.18, 0.16, 0.22, 0.25, 0.26, 0.28, 0.31, 0.33, 0.36, 0.40, 0.45};
double noise_ch6[NUM_RUNS] = {0.00, 0.02, 0.07, 0.10, 0.12, 0.14, 0.17, 0.19, 0.17, 0.23, 0.25, 0.28, 0.30, 0.32, 0.35, 0.39, 0.45, 0.57};
double noise_ch7[NUM_RUNS] = {0.00, 0.02, 0.08, 0.12, 0.14, 0.17, 0.21, 0.23, 0.21, 0.29, 0.32, 0.34, 0.38, 0.41, 0.46, 0.52, 0.61, 0.84};

/*
// OLD
double cross_talk_ch4[NUM_RUNS] = {6.2, 19.6, 10.0, 6.90, 8.10, 9.70, 11.3, 14.9, 12.1, 20.1, 22.9, 27.3, 30.1, 34.3, 39.2, 45.2, 49.4, 55.9};
double cross_talk_ch5[NUM_RUNS] = {0.0, 26.8, 7.60, 7.10, 8.30, 9.40, 11.9, 14.1, 11.3, 19.5, 22.2, 25.5, 30.2, 33.3, 38.2, 43.0, 47.6, 53.8};
double cross_talk_ch6[NUM_RUNS] = {0.0, 21.5, 8.70, 7.20, 8.10, 9.60, 12.1, 14.8, 12.0, 20.8, 24.1, 28.2, 33.4, 38.6, 43.4, 50.6, 57.2, 65.3};
double cross_talk_ch7[NUM_RUNS] = {0.0, 21.2, 7.20, 6.40, 7.50, 9.20, 11.1, 13.6, 11.4, 19.7, 23.9, 28.9, 33.8, 39.3, 45.4, 52.4, 60.7, 71.1};
*/

double cross_talk_ch4[NUM_RUNS] = {1.6, 14.1, 5.5, 5.8, 6.6, 7.5, 8.9, 11.8, 9.1, 14.8, 16.8, 20.1, 20.3, 24.4, 26.2, 31.4, 34.9, 41.1};
double cross_talk_ch5[NUM_RUNS] = {0.0, 24.2, 7.1, 6.3, 7.2, 7.9, 9.4, 10.9, 9.0, 14.4, 16.5, 18.3, 20.8, 23.3, 26.1, 29.3, 33.4, 40.0};
double cross_talk_ch6[NUM_RUNS] = {0.0, 18.3, 6.7, 6.4, 6.7, 7.2, 9.1, 10.9, 8.5, 14.1, 15.1, 17.6, 20.1, 23.9, 27.7, 33.3, 39.8, 49.3};
double cross_talk_ch7[NUM_RUNS] = {0.0, 18.5, 6.0, 5.3, 6.0, 6.5, 7.8, 9.10, 7.6, 12.1, 13.3, 16.9, 18.7, 21.8, 26.8, 33.1, 41.9, 55.9};


void make_bias_array(); // fills the bias array w. values
void make_pe_distribution_array(int feb, int channel); // fills the pe_distributiion array w. distribution TF1* plots
void make_mpv_array(); // fills the mpv array w. mpv values of the distribution plots
void make_fwhm_array(); // fills the fwhm array w. fwhm values of the distribution plots
void make_fwhm_div_mpv_array(); // fills the fwhm_div_mpv array w. fwhm:mpv ratios

void make_noise_arrays();
void make_cross_talk_arrays();

void make_correlation_plot(double* bias_arr, double* y_arr, const char* title, const char* y_title, const char* file_name); // creates different bias plots
void make_all_correlation_plots_1(int feb, int channel); // makes all the desired bias plots
void make_all_correlation_plots_2();

int get_run_num_from_tree(TTree* run);
TTree* get_tree_from_run(int run_num);
TF1* get_pe_distribution(TTree* run, int feb, int channel, double bias_i);
double get_mpv(TF1* distribution); // most probable value
double get_fwhm(TF1* distribution); // full-width half-max
void set_canvas_style();

void Bias_Study_Configuration_2(){
    make_bias_array();
    
    /*
    // 4, 5, 6, or 7
    int channel = 7;
    
    make_pe_distribution_array(0,channel);
    make_mpv_array();
    make_fwhm_array();
    make_fwhm_div_mpv_array();
    
    make_all_correlation_plots_1(0,channel);
     */
    
    make_all_correlation_plots_2();
}

void make_all_correlation_plots_1(int feb, int channel){
    // single channel
    make_correlation_plot(bias,mpv,Form("[%d][%d] Most Probable PE Value vs. Bias Voltage",feb,channel),"Most Probable Value (PE)",Form("[%d][%d]_mpv_vs_bias",feb,channel));
    make_correlation_plot(bias,fwhm,Form("[%d][%d] FWHM vs. Bias",feb,channel),"Full-Width at Half-Max (PE)",Form("[%d][%d]_fwhm_vs_bias",feb,channel));
    make_correlation_plot(bias,fwhm_div_mpv,Form("[%d][%d] FWHM to MPV vs. Bias",feb,channel),"FWHM to MPV",Form("[%d][%d]_fwhm_to_mpv_vs_bias",feb,channel));
}

void make_all_correlation_plots_2() {
    /*
    make_correlation_plot(bias,noise_ch4,"[0][4] Noise Rate vs. Bias Voltage","Noise Rate (MHz)","[0][4]_noise_vs_bias");
    make_correlation_plot(bias,noise_ch5,"[0][5] Noise Rate vs. Bias Voltage","Noise Rate (MHz)","[0][5]_noise_vs_bias");
    make_correlation_plot(bias,noise_ch6,"[0][6] Noise Rate vs. Bias Voltage","Noise Rate (MHz)","[0][6]_noise_vs_bias");
    make_correlation_plot(bias,noise_ch7,"[0][7] Noise Rate vs. Bias Voltage","Noise Rate (MHz)","[0][7]_noise_vs_bias");
     */
    
    make_correlation_plot(bias,cross_talk_ch4,"[0][4] Cross Talk Probability vs. Bias Voltage","Cross Talk Probability (%)","[0][4]_cross_talk_vs_bias");
    make_correlation_plot(bias,cross_talk_ch5,"[0][5] Cross Talk Probability vs. Bias Voltage","Cross Talk Probability (%)","[0][5]_cross_talk_vs_bias");
    make_correlation_plot(bias,cross_talk_ch6,"[0][6] Cross Talk Probability vs. Bias Voltage","Cross Talk Probability (%)","[0][6]_cross_talk_vs_bias");
    make_correlation_plot(bias,cross_talk_ch7,"[0][7] Cross Talk Probability vs. Bias Voltage","Cross Talk Probability (%)","[0][7]_cross_talk_vs_bias");
}

void make_bias_array(){
    for(int i=0; i<NUM_RUNS; i++){
        bias[i] = 52.1+0.5*i;
    }
}

void make_pe_distribution_array(int feb, int channel){
    if(channel%4 <= 1){
        // y = 25mm
        pe_distribution[0] = get_pe_distribution(get_tree_from_run(1142),feb,channel,bias[0]);
        pe_distribution[1] = get_pe_distribution(get_tree_from_run(1144),feb,channel,bias[1]);
        pe_distribution[2] = get_pe_distribution(get_tree_from_run(1147),feb,channel,bias[2]);
        pe_distribution[3] = get_pe_distribution(get_tree_from_run(1148),feb,channel,bias[3]);
        pe_distribution[4] = get_pe_distribution(get_tree_from_run(1151),feb,channel,bias[4]);
        pe_distribution[5] = get_pe_distribution(get_tree_from_run(1152),feb,channel,bias[5]);
        pe_distribution[6] = get_pe_distribution(get_tree_from_run(1156),feb,channel,bias[6]);
        pe_distribution[7] = get_pe_distribution(get_tree_from_run(1157),feb,channel,bias[7]);
        pe_distribution[8] = get_pe_distribution(get_tree_from_run(1160),feb,channel,bias[8]);
        pe_distribution[9] = get_pe_distribution(get_tree_from_run(1164),feb,channel,bias[9]);
        pe_distribution[10] = get_pe_distribution(get_tree_from_run(1174),feb,channel,bias[10]);
        pe_distribution[11] = get_pe_distribution(get_tree_from_run(1173),feb,channel,bias[11]);
        pe_distribution[12] = get_pe_distribution(get_tree_from_run(1170),feb,channel,bias[12]);
        pe_distribution[13] = get_pe_distribution(get_tree_from_run(1177),feb,channel,bias[13]);
        pe_distribution[14] = get_pe_distribution(get_tree_from_run(1178),feb,channel,bias[14]);
        pe_distribution[15] = get_pe_distribution(get_tree_from_run(1179),feb,channel,bias[15]);
        pe_distribution[16] = get_pe_distribution(get_tree_from_run(1180),feb,channel,bias[16]);
        pe_distribution[17] = get_pe_distribution(get_tree_from_run(1181),feb,channel,bias[17]);
    }
    else{
        // y = 75mm
        pe_distribution[0] = get_pe_distribution(get_tree_from_run(1140),feb,channel,bias[0]);
        pe_distribution[1] = get_pe_distribution(get_tree_from_run(1145),feb,channel,bias[1]);
        pe_distribution[2] = get_pe_distribution(get_tree_from_run(1146),feb,channel,bias[2]);
        pe_distribution[3] = get_pe_distribution(get_tree_from_run(1149),feb,channel,bias[3]);
        pe_distribution[4] = get_pe_distribution(get_tree_from_run(1150),feb,channel,bias[4]);
        pe_distribution[5] = get_pe_distribution(get_tree_from_run(1153),feb,channel,bias[5]);
        pe_distribution[6] = get_pe_distribution(get_tree_from_run(1155),feb,channel,bias[6]);
        pe_distribution[7] = get_pe_distribution(get_tree_from_run(1158),feb,channel,bias[7]);
        pe_distribution[8] = get_pe_distribution(get_tree_from_run(1159),feb,channel,bias[8]);
        pe_distribution[9] = get_pe_distribution(get_tree_from_run(1163),feb,channel,bias[9]);
        pe_distribution[10] = get_pe_distribution(get_tree_from_run(1175),feb,channel,bias[10]);
        pe_distribution[11] = get_pe_distribution(get_tree_from_run(1172),feb,channel,bias[11]);
        pe_distribution[12] = get_pe_distribution(get_tree_from_run(1171),feb,channel,bias[12]);
        pe_distribution[13] = get_pe_distribution(get_tree_from_run(1176),feb,channel,bias[13]);
        pe_distribution[14] = get_pe_distribution(get_tree_from_run(1182),feb,channel,bias[14]);
        pe_distribution[15] = get_pe_distribution(get_tree_from_run(1183),feb,channel,bias[15]);
        pe_distribution[16] = get_pe_distribution(get_tree_from_run(1184),feb,channel,bias[16]);
        pe_distribution[17] = get_pe_distribution(get_tree_from_run(1186),feb,channel,bias[17]);
    }
}

void make_mpv_array(){
    for(int i=0;i<NUM_RUNS;i++){
        mpv[i] = get_mpv(pe_distribution[i]);
    }
}

void make_fwhm_array(){
    for(int i=0;i<NUM_RUNS;i++){
        fwhm[i] = get_fwhm(pe_distribution[i]);
    }
}

void make_fwhm_div_mpv_array(){
    for(int i=0;i<NUM_RUNS;i++){
        if(mpv[i] <= 0)
            fwhm_div_mpv[i] = 0;
        else
            fwhm_div_mpv[i] = fwhm[i]/mpv[i];
    }
}

void make_correlation_plot(double* bias_arr, double* y_arr, const char* title, const char* y_title, const char* file_name){
    TGraphErrors* graph_1 = new TGraphErrors(NUM_RUNS,bias_arr,y_arr,nullptr,nullptr);
    graph_1->SetTitle(title);
    graph_1->SetMarkerStyle(8);
    graph_1->SetMarkerColor(kBlack);
    graph_1->SetLineStyle(7);
    graph_1->SetLineColor(kBlack);
    
    // graph_1->GetXaxis()->SetRangeUser(51,62);
    graph_1->GetXaxis()->SetTitle("Bias Voltage [V]");
    graph_1->GetXaxis()->SetTitleOffset(1.2);
    graph_1->GetXaxis()->SetTitleSize(0.05);
    graph_1->GetXaxis()->SetLabelSize(0.05);
    graph_1->GetXaxis()->SetLabelOffset(0.015);
    graph_1->GetYaxis()->SetTitle(y_title);
    graph_1->GetYaxis()->SetTitleOffset(1.2);
    graph_1->GetYaxis()->SetTitleSize(0.05);
    graph_1->GetYaxis()->SetLabelSize(0.05);
    
    TCanvas* canvas_1 = new TCanvas();
    set_canvas_style();
    graph_1->Draw();
    
    string s(file_name);
    
    string out_name = "Bias_Correlation_Plots/"+s;
    canvas_1->Print(Form("%s.png",out_name.c_str()),"png");
    
    TFile* f_out = new TFile(Form("%s.root",out_name.c_str()),"RECREATE");
    graph_1->Write("bias");
    f_out->Close();
}

int get_run_num_from_tree(TTree* run){
    string s(run->GetName());
    return stoi(s.substr(3,4));
}

TTree* get_tree_from_run(int run_num){
    gSystem->cd("../");
    TFile* file;
    if(run_num < 1000)
        file = new TFile(Form("Run_0%dmerged.root", run_num),"READ");
    else
        file = new TFile(Form("Run_%dmerged.root", run_num),"READ");
    gSystem->cd("Bias_Study_Configuration_2");
    if(file == NULL)
        return NULL;
    if(file->GetListOfKeys()->Contains(Form("run%d", run_num))){
        TTree* run = (TTree*)file->Get(Form("run%d", run_num));
        if(run->GetEntries() < 3000)
            return NULL;
        return run;
    }
    return NULL;
}

TF1* get_pe_distribution(TTree* run, int feb, int channel, double bias_i){
    TCanvas* canvas_1 = new TCanvas();
    set_canvas_style();
    
    // single channel
    char* bounds;
    if(channel%4 <= 1){
        bounds = Form("PEs_[%d][%d] > 30 && valid_[%d][%d] && position_y_[%d][%d] > 15 && position_y_[%d][%d] < 35",feb,channel,feb,channel,feb,channel,feb,channel);
    }
    else{
        bounds = Form("PEs_[%d][%d] > 30 && valid_[%d][%d] && position_y_[%d][%d] > 65 && position_y_[%d][%d] < 85",feb,channel,feb,channel,feb,channel,feb,channel);
    }
    run->Draw(Form("PEs_[%d][%d] >> pe_distr_1(%f,%f,%f)",feb,channel,X_MAX-X_MIN,X_MIN,X_MAX),bounds);
    
    TH1F* pe_distr_1 = (TH1F*) gDirectory->FindObjectAny("pe_distr_1");
    pe_distr_1->GetXaxis()->SetRangeUser(X_MIN,X_MAX);
    pe_distr_1->GetXaxis()->SetTitle("Number of PEs");
    pe_distr_1->GetXaxis()->SetTitleOffset(1.2);
    pe_distr_1->GetXaxis()->SetTitleSize(0.05);
    pe_distr_1->GetXaxis()->SetLabelSize(0.05);
    pe_distr_1->GetXaxis()->SetLabelOffset(0.015);
    pe_distr_1->GetYaxis()->SetTitle("Count");
    pe_distr_1->GetYaxis()->SetRangeUser(0,2500);
    pe_distr_1->GetYaxis()->SetTitleOffset(1.2);
    pe_distr_1->GetYaxis()->SetTitleSize(0.05);
    pe_distr_1->GetYaxis()->SetLabelSize(0.05);
    
    int maxbin = pe_distr_1->GetMaximumBin();
    int midPE = pe_distr_1->GetBinCenter(maxbin);
    TF1* gau = new TF1("gau","gaus",X_MIN,midPE);
    TF1* lan = new TF1("lan","landau",midPE,X_MAX);
    pe_distr_1->Fit(gau,"0QR");     // 0 : do not plot the result of the fit
    pe_distr_1->Fit(lan,"0QR+");    // Q : quiet mode (min printing)
    Double_t par[6];                // R : use range specified in the function range
    gau->GetParameters(&par[0]);    // + : add this fitted function to the list of fitted funtions
    lan->GetParameters(&par[3]);    //     (by default, any previous fitted function is deleted)
    // TF1 *lgau = new TF1("lgau","gaus(0)+landau(3)",X_MIN,X_MAX);
    TF1 *lgau = new TF1("lgau","gaus(0)+landau(3)",midPE-50,midPE+80);
    lgau->SetParameters(par);
    lgau->SetLineColor(kGreen+2);
    pe_distr_1->Fit(lgau, "QR+");
    
    TLatex* text_1 = new TLatex(170,1800,Form("#splitline{Run %d}{Bias = %3.1f}",get_run_num_from_tree(run),bias_i));
    text_1->Draw();
    
    canvas_1->Print(Form("PE_Distribution_Plots/[%d][%d]_run%d.png",feb,channel,get_run_num_from_tree(run)),"png");
    
    canvas_1->Close();
    return lgau;
}

double get_mpv(TF1* distribution){
    return distribution->GetMaximumX(X_MIN,X_MAX);
}

double get_fwhm(TF1* distribution){
    double most_probable_value = distribution->GetMaximumX();
    double half_max = distribution->GetMaximum(X_MIN,X_MAX)/2;
    double x_max = -999, x_min = 999;
    int npts = 1000;
    for(int i=0;i<npts;i++){
        double x = X_MAX*i/npts;
        double y = distribution->Eval(x);
        if(y < half_max && x < most_probable_value)
            x_min = x;
        if(y > half_max && x > most_probable_value)
            x_max = x;
    }
    double full_width_half_max = x_max - x_min;
    if(full_width_half_max > 0)
        return full_width_half_max;
    else
        return 0;
}

void set_canvas_style(){
    gStyle->SetOptStat("0");
    gStyle->SetTitleSize(0.075,"t");
    gPad->SetLeftMargin(0.125);
    gPad->SetBottomMargin(0.125);
}
