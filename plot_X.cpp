#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TBox.h>
#include <TLegend.h>
#include <iostream>
#include <TStyle.h>

#include "ACCSELX.h"

void plot_X() {



// MC FILEs
const char * files[] = {//MC FILEs
   // "Bu_phat5_Bfinder.root"     //ppRef                         
    //"Bd_phat5_Bfinder.root"       //ppRef
    //"Bs_phat5_Bfinder.root"       //ppRef
   "/lstore/cms/henrique/X3872/MC_DATA/prompt_PSI2S_to_Jpsi_pipi_phat5_Bfinder.root"   
           
};
const char* filesx={  "/lstore/cms/henrique/X3872/MC_DATA/prompt_X3872_to_Jpsi_Rho_phat5_Bfinder.root" };//MC FILEs X3872

const char* files_data={//REAL DATA
     "/lstore/cms/henrique/X3872/MC_DATA/DATA_ppRef_X3872.root"
};

//VARIABLES
//PSI2S variable ranges
//const char * variables[] = {"Balpha",  "BQvalueuj"    ,  "Bcos_dtheta", "BtrkPtimb",  "Bchi2cl",  "Btrk1dR",    "Btrk2dR",   "Btrk1Pt",   "Btrk2Pt", "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance" , "Bnorm_trk1Dxy"  , "Bnorm_trk2Dxy", "Btktkmass", "Bpt"  ,       "By"    , "nSelectedChargedTracks"};
//const double ranges[][2] = {{0,3.2},   {0,1.05}       ,   {0,1.05}      ,    {0,0.9},    {0,1.02}, {0,3.2},      {0,3} ,   {0, 4.6} , {0, 2.25},           {0,16.5},          {0,21.5}   ,         {-13,29}  ,          {-13,29}   ,  {0.2,2}  ,   {0, 20}  ,    {-2.3, 2.3}    ,   {0,136}};
//X3872 variable ranges

//const char * variables[] = {"Balpha",  "BQvalueuj"    ,  "Bcos_dtheta", "BtrkPtimb",  "Bchi2cl",  "Btrk1dR",    "Btrk2dR",   "Btrk1Pt",   "Btrk2Pt", "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance" , "Bnorm_trk1Dxy"  , "Bnorm_trk2Dxy", "Btktkmass", "Bpt"  ,       "By"    , "nSelectedChargedTracks"};
//const double ranges[][2] = {{0,3.2},   {0,1.3}       ,   {0,1.05}      ,    {0,0.9},    {0,1.02},   {0,3.3},     {0,3.3} ,   {0, 2.5} , {0, 3.7},           {0,8},          {0,8}   ,              {-36,44}  ,          {-15,34}   ,  {0.2,1.2}  ,   {0, 21.5}  ,    {-2.3, 2.5}    ,   {0,115}};

//full variable ranges
//const char * variables[] = {"Balpha",  "BQvalueuj"    ,  "Bcos_dtheta", "BtrkPtimb",  "Bchi2cl",  "Btrk1dR",    "Btrk2dR",   "Btrk1Pt",   "Btrk2Pt", "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance" , "Bnorm_trk1Dxy"  , "Bnorm_trk2Dxy", "Btktkmass", "Bpt"  ,       "By"    , "nSelectedChargedTracks"};
//const double ranges[][2] = {{0,3.2},   {0,1.3}       ,   {0,1.05}      ,    {0,0.9},    {0,1.02},       {0,3.3},  {0,3.3} ,   {0, 4.6} ,  {0, 3.7},           {0,16.5},          {0,21.5}   ,         {-36,44}  ,          {-15,34}   ,  {0.2,2}  ,   {0, 21.5}  ,    {-2.3, 2.5}    ,   {0,136}};


int SELplots = 1; //mudar para 1 com ruído e descomentar a linha acima 


//VARIABLES
//const char * variables[] = {  "Btrk1dR", "Btrk2dR"};
//const double ranges[][2] = {   {0,4.5},    {0,2} };

const char * variables[] = {"Bmass"};//comentar o Btkmass
const double ranges[][2] = {3.6 , 4};
//VARIABLES


TString cutlevel = ""; // "_RAW", "_ACC", "_SEL", "_TRG", "", 


TString path_to_file = "";

const int nVars = sizeof(variables)/sizeof(variables[0]);

for (int ifile = 0; ifile < sizeof(files)/sizeof(files[0]); ++ifile) {
    path_to_file = Form("%s", files[ifile]);
   
  

    TFile *file = TFile::Open(path_to_file.Data());
    TFile *file_data = TFile::Open(files_data);
    TFile*filex = TFile::Open(filesx);
    // Get the trees from the file
    TTree *treeMix;
    TTree *treedata;
    TTree *treex;


    if(path_to_file.Contains("X3872")){ 

        file->GetObject("Bfinder/ntmix", treeMix);//PSI2S  
        filex->GetObject("Bfinder/ntmix", treex);//X3872                                       
        file_data->GetObject("Bfinder/ntmix", treedata);
    }


    std::cout << "\n" << "Entries in treeMix: " << treeMix->GetEntries() << std::endl;
    std::cout << "\n" << "Entries in treedata: " << treedata->GetEntries() << std::endl;
    std::cout << "\n" << "Entries in treex: " << treex->GetEntries() << std::endl;
    for (int i = 0; i < nVars; ++i) {
        TString var = variables[i];

        // Create a canvas to draw the histograms
        TCanvas *canvas = new TCanvas("canvas", "", 600, 600);
        canvas->SetLeftMargin(0.15);
        canvas->SetTopMargin(0.05);
        canvas->SetRightMargin(0.05); 

        double hist_Xhigh      = ranges[i][1];
        double hist_Xlow       = ranges[i][0];
        int hist_Nbin          =  150;//(hist_Xhigh - hist_Xlow)/0.005;//
        if (var == "nSelectedChargedTracks") {
            hist_Nbin = hist_Xhigh - hist_Xlow;
        } 
        double bin_length_MEV  = (hist_Xhigh - hist_Xlow) / hist_Nbin;
        if(SELplots){ hist_Nbin = 150; } // -----------------------------------------> NUMERO DE BINs PARA PLOTAR COM RUÍDO
        
        TString Xlabel ;
        if (var == "Bmass"){ 
        if(path_to_file.Contains("PSI2S")){
                Xlabel="m_{J/#Psi#pi^{+}#pi^{-}} [GeV/c^{2}]";
            } else {
                Xlabel = "m_{J/#Psi K^{+}} [GeV/c^{2}]";
            }
        } else if (var == "Bpt"){ 
            Xlabel = "p_{T} [GeV/c]";
        } else { 
            Xlabel = var.Data();
        }

        // Create histograms
        TH1F *hist_SIG = new TH1F("hist_SIG"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist_sig = new TH1F("hist_sig"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh); 
        TH1F *hist_BKG = new TH1F("hist_BKG"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist     = new TH1F("hist"          , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
       // TH1F *hist_SIG_WT   = new TH1F("hist_SIG_WT"  , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        
        //TH1F *hist_SIG_BOTH = new TH1F("hist_SIG_BOTH", Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        

        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //SELECT THE acc + presel CUT 

        TString dirNAME = "presel_STUDY/";
        TString Final = "1";      
        TString trgmatches = TRGmatching.Data();   //TRG matching only in ppRef
        TString ACCcuts = "" ;
        TString SELcuts = "" ;
       
            ACCcuts    = ACCcuts_ppRef.Data(); //ppRef
            SELcuts    = SELcuts_ppRef.Data(); //ppRef
            if (path_to_file.Contains("PbPb")) { 
                ACCcuts = ACCcuts_PbPb.Data();
                SELcuts = SELcuts_PbPb.Data();
                trgmatches = "1";
            }
        

        TString cut = "";
        if (cutlevel == "_RAW")       {cut = Form(" %s "                   ,FIDreg.Data());}                                                              //RAW (inside fid reg only)
        else if (cutlevel == "_ACC")  {cut = Form(" %s && %s "             ,FIDreg.Data(), ACCcuts.Data());}                                              //ACC
        else if (cutlevel == "_SEL")  {cut = Form(" %s && %s && %s "       ,FIDreg.Data(), ACCcuts.Data(), SELcuts.Data());}                              //SEL
        else if (cutlevel == "_TRG")  {cut = Form(" %s && %s && %s && %s " ,FIDreg.Data(), ACCcuts.Data(), SELcuts.Data(), trgmatches.Data());}           //TRG
        else if (cutlevel == ""){
            if (!SELplots) {dirNAME  = "";}
            cut = Form(" %s && %s && %s && %s", ACCcuts.Data(), SELcuts.Data(), trgmatches.Data(), Final.Data());                   //Final
        }
        else{
            std::cerr << "Invalid cut level specified: " << cutlevel << std::endl;
            return;
        }                                                                                                 

        TString sepcCASES = "1";
      

        if(path_to_file.Contains("X3872")){ // X3872{
            if (var== "Bmass") {
                treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form(" (Bchi2cl > 0.02) && (BQvalueuj < 0.2) &&  %s && %s", cut.Data(),sepcCASES.Data()));    //(Bchi2cl > 0.02) &&  
        }else{ 
            treeMix->Draw(Form("%s >> hist_SIG", var.Data()), Form("(Bchi2cl > 0.02) &&  (Bnorm_svpvDistance_2D < 1.3) && %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));//PSI2S (Bchi2cl > 0.02) && 
            treex->Draw(Form("%s >> hist_sig", var.Data()), Form(" (Bchi2cl > 0.02) &&  (Bnorm_svpvDistance_2D < 1.3) && %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data())); // X3872 (Bchi2cl > 0.02) &&
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("((Bmass < 3.65771 ) || (Bmass >  3.71717 && Bmass <  3.83576) || (Bmass > 3.91064)) &&  (Bchi2cl > 0.02) &&  (Bnorm_svpvDistance_2D < 1.3) && %s && %s", cut.Data(),sepcCASES.Data())); //   (Bchi2cl > 0.02) && 
        } }
        //SELECT THE acc + presel CUT     
        // Customize the Histograms
        hist->SetLineColor(kBlack);
        hist->SetLineWidth(2);

        hist_SIG->SetLineColor(kOrange+7);
        hist_SIG->SetFillColor(kOrange+7);    
        //hist_SIG->SetFillStyle(3001); 
        //hist_SIG->SetLineWidth(2);
        //hist_SIG->SetLineStyle(2);


        hist_BKG->SetLineColor(kBlue);
        hist_BKG->SetFillColor(kBlue);     
        hist_BKG->SetFillStyle(3358);
        //hist_BKG->SetMaximum(1.1 * hist_BKG->GetMaximum()); // Increase the max range to give some space
        //hist_BKG->SetLineStyle(2);
        //hist_BKG->SetLineWidth(2);

        hist_sig->SetLineColor(kOrange+7);
        hist_sig->SetFillColor(kOrange+7);
        //hist_sig->SetFillStyle(3001);
        //hist_BKG->SetMarkerStyle(20); // Circle marker
        //hist_BKG->SetMarkerSize(.8); // Bigger dots
        // Customize the Histograms

        hist_BKG->SetName("DATA_BKG");  // <--- Also affects the stat box

        if(path_to_file.Contains("X3872")){
            hist_sig->SetName("MC_SIG_X3872");
            hist_SIG->SetName("MC_SIG_PSI2S");
                }
        if (SELplots == 1) { // NORMALIZE
              double int_sig     = hist_SIG->Integral();
              double int_sig_x   = hist_sig->Integral();
              double int_bkg     = hist_BKG->Integral();
            

            if (int_sig > 0 ||int_bkg > 0){
                hist_SIG->Scale(1.0 / int_sig);
                hist_BKG->Scale(1.0 / int_bkg);
                hist_sig->Scale(1.0 / int_sig_x);
          }
        } 

        if(1){// set the y-axis maximum if needed
            Double_t     max_val = TMath::Max(hist->GetMaximum(), TMath::Max(hist_BKG->GetMaximum(), hist_SIG->GetMaximum()));
            if(SELplots) {
                if (path_to_file.Contains("X3872")) {
                    max_val =TMath::Max( hist_BKG->GetMaximum(), hist_SIG->GetMaximum()); ;
                    hist_SIG->SetMaximum(max_val * 1.1);  // the max is set by PSI because it is bigger
                
                }
                hist_BKG->SetMaximum(max_val * 1.1); 
            } 
        }

        // Draw the histograms
         hist->SetStats(0);
         if(SELplots){
            if(var=="Bmass"){
                hist_BKG->Draw("HIST");
            }else{
                hist_SIG->Draw("HIST");
                hist_sig->Draw("HIST SAMES");
                hist_BKG->Draw("HIST SAMES");
            
         }
       
         }

        if(!SELplots) hist->Draw("HIST SAME");
        canvas->Update();
        gPad->Update();

        // Move and color the stat boxes
        TPaveStats *st_bkg = (TPaveStats*)hist_BKG->GetListOfFunctions()->FindObject("stats");
        if (st_bkg) {
            st_bkg->SetTextColor(kBlue);
            st_bkg->SetLineColor(kBlue); 
            st_bkg->SetX1NDC(0.75);
            st_bkg->SetX2NDC(0.95);
            st_bkg->SetY1NDC(0.85);
            st_bkg->SetY2NDC(0.95);
            st_bkg->Draw();
        }////////////////////////////////////////////////////////////////////////////////////  PSI
       TPaveStats *st_sig = (TPaveStats*)hist_SIG->GetListOfFunctions()->FindObject("stats");
        if (st_sig) {
            st_sig->SetTextColor(kOrange+7);
            st_sig->SetLineColor(kOrange+7);
            st_sig->SetX1NDC(0.75);
            st_sig->SetX2NDC(0.95);
            st_sig->SetY1NDC(0.75);
            st_sig->SetY2NDC(0.85);
            st_sig->Draw();
        }////////////////////////////////////////////////////////////////////////////////// X3872
        TPaveStats *st_sigx = (TPaveStats*)hist_sig->GetListOfFunctions()->FindObject("stats");
        if (st_sig) {
            st_sigx->SetTextColor(kOrange+7);
            st_sigx->SetLineColor(kOrange+7);
            st_sigx->SetX1NDC(0.75);
            st_sigx->SetX2NDC(0.95);
            st_sigx->SetY1NDC(0.65);
            st_sigx->SetY2NDC(0.75);
            st_sigx->Draw();
        }
     
        // LATEX text
        if(0){
            double Nsignal = hist_SIG->GetEntries();
            double Nbkg = hist_BKG->GetEntries();
            double significance = (Nbkg > 0) ? Nsignal / sqrt(Nbkg) : 0;

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.022);
            latex.SetTextColor(kOrange+7); // Same as hist_SIG
            latex.DrawLatex(0.18, 0.82, Form("N_{sig} = %.0f", Nsignal));
            latex.SetTextColor(kBlue);     // Same as hist_BKG
            latex.DrawLatex(0.18, 0.85, Form("N_{bkg} = %.0f", Nbkg));
            latex.SetTextColor(kOrange+7); 
            latex.DrawLatex(0.18, 0.68, Form("N_{sig X3872} = %.0f", hist_sig->GetEntries()));
        }
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // Add a legend
        auto legend = new TLegend(0.15, 0.7, 0.25, 0.9);
        legend->AddEntry(hist_SIG, "MC SIG PSI", "l");
        legend->AddEntry(hist_BKG, "MC BKG", "l");
        legend->AddEntry(hist_sig, "MC SIG X3872", "l");
              
        // Save the canvas as an image
        TString particleNAME1 = " ";
        TString particleNAME2 = " ";
        TString systemNAME = "MC_ppRef_";
    
        
         if(path_to_file.Contains("PSI2S")){
            particleNAME1="PSI2S";
            particleNAME2="X3872";}
        if (path_to_file.Contains("PbPb"))  { systemNAME = "MC_PbPb_";}

       // canvas->SaveAs(Form("./%s%s%s_%s%s_btrk2dr_alpha0.05_nb150.pdf", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME.Data(), cutlevel.Data()));
        canvas->SaveAs(Form("./%s%s%s_%s_%s%scut2t2.pdf", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME1.Data(), particleNAME2.Data(), cutlevel.Data()));
        //canvas->SaveAs(Form("./%s%s%s_%s%s_optimalcut.root", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME.Data(), cutlevel.Data()));


        // Clean up
        delete hist_SIG;
        delete hist_sig;
        delete hist_BKG;
        delete hist;
        delete canvas;
        
    }

}
}
    

int main() {
    plot_X();
    return 0;
}