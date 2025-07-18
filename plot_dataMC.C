#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TBox.h>
#include <TLegend.h>
#include <iostream>
#include <TStyle.h>

#include "ACCSEL.h"

void plot_dataMC() {

// MC FILEs
const char * files[] = {
   // "Bu_phat5_Bfinder.root",     //ppRef                         
    "Bd_phat5_Bfinder.root"       //ppRef
    //"Bs_phat5_Bfinder.root"
           
};

const char* files_data={
    "/lstore/cms/henrique/Bmeson/MC_DATA/DATA_ppref_Bmeson/DATA_ppref_Bmeson.root" // Real data
};

//VARIABLES
//VARIABLES
//const char * variables[] = {/*"Balpha", "BQvalueuj" ,  "Bcos_dtheta", "BtrkPtimb" , "Bchi2cl" , "Btrk1dR", "Btrk2dR" , "Btrk1Pt", "Btrk2Pt" ,*/ "Bnorm_svpvDistance_2D"/*, "Bnorm_svpvDistance" , "Bnorm_trk1Dxy" ,  "Bnorm_trk2Dxy"*/};
//const double ranges[][2] = {/*{0,3.15}  , {0,2.5}     ,      {0,1} ,      {0,1}  , {0.05,1}  ,     {0,2}  ,  {0,2}  ,     {0.5, 10} , {0.5, 10},  */  {0,85} /*,            {0,85}   ,       {-22,22}  ,          {-22,22} */};
int SELplots = 1; //mudar para 1 com ruído e descomentar a linha acima

const char * variables[] = {"Bmass" /*, "Btktkmass", "Bpt"  ,       "By"    , "nSelectedChargedTracks"};//*/};//comentar o Btkmass
const double ranges[][2] = {{5 , 6} /*,   {0,2.5}  , {5, 50}  , {-2.4, 2.4}  ,   {0,150}*/};
//VARIABLES
//VARIABLES

/////////////////////////////////  ///////////////////////////  ////////////////

TString cutlevel = ""; // "_RAW", "_ACC", "_SEL", "_TRG", "", 

/////////////////////////////////  ///////////////////////////  ///////////

TString path_to_file = "";

const int nVars = sizeof(variables)/sizeof(variables[0]);

for (int ifile = 0; ifile < sizeof(files)/sizeof(files[0]); ++ifile) {
    path_to_file = Form("%s", files[ifile]);
    //path_to_file = Form("/eos/user/h/hmarques/MC_ppRef_Bmeson/MC_ppRef_Bmeson/%s_Bfinder.root", files[ifile]);

     

    TFile *file = TFile::Open(path_to_file.Data());
    TFile *file_data = TFile::Open(files_data);
    // Get the trees from the file
    TTree *treeMix;
    TTree *treedata;
    if (path_to_file.Contains("Bs")){
        file->GetObject("Bfinder/ntphi", treeMix);
        file_data->GetObject("Bfinder/ntphi", treedata);
    } else if (path_to_file.Contains("Bd")){
        file->GetObject("Bfinder/ntKstar", treeMix);
        file_data->GetObject("Bfinder/ntKstar", treedata);
    } else {
        file->GetObject("Bfinder/ntKp", treeMix);
        file_data->GetObject("Bfinder/ntKp", treedata);
    }


    std::cout << "\n" << "Entries in treeMix: " << treeMix->GetEntries() << std::endl;
    std::cout << "\n" << "Entries in treedata: " << treedata->GetEntries() << std::endl;

    for (int i = 0; i < nVars; ++i) {
        TString var = variables[i];

        if(path_to_file.Contains("Bu") && ((var.Contains("trk2") || var.Contains("Ptimb")))) continue; // B+ has less one track!

        // Create a canvas to draw the histograms
        TCanvas *canvas = new TCanvas("canvas", "", 600, 600);
        canvas->SetLeftMargin(0.15);
        canvas->SetTopMargin(0.05);
        canvas->SetRightMargin(0.05); 

        double hist_Xhigh      = ranges[i][1];
        double hist_Xlow       = ranges[i][0];
        int hist_Nbin          = 150 ;
        if (var == "nSelectedChargedTracks") {
            hist_Nbin = hist_Xhigh - hist_Xlow;
        } 
        double bin_length_MEV  = (hist_Xhigh - hist_Xlow) / hist_Nbin;
        if(SELplots){ hist_Nbin = 50; }
        
        TString Xlabel ;
        if (var == "Bmass"){ 
            if (path_to_file.Contains("Bs")){
                Xlabel = "m_{J/#Psi K^{+} K^{-}} [GeV/c^{2}]";
            } else if (path_to_file.Contains("Bd")){
                Xlabel = "m_{J/#Psi K^{+} #pi^{-}} [GeV/c^{2}]";
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
        TH1F *hist_BKG = new TH1F("hist_BKG"      , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist     = new TH1F("hist"          , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);
        TH1F *hist_SIG_WT   = new TH1F("hist_SIG_WT"  , Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        
        TH1F *hist_SIG_BOTH = new TH1F("hist_SIG_BOTH", Form("; %s; Entries / %.3f ", Xlabel.Data(), bin_length_MEV) , hist_Nbin, hist_Xlow ,hist_Xhigh);        

        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //SELECT THE acc + presel CUT 

        TString dirNAME = "presel_STUDY/";
        TString Final = "1";      
        TString trgmatches = TRGmatching.Data();   //TRG matching only in ppRef
        TString ACCcuts = "" ;
        TString SELcuts = "" ;

        if (path_to_file.Contains("Bu")){
            ACCcuts    = ACCcuts_ppRef_Bu.Data(); //ppRef
            SELcuts    = SELcuts_ppRef_Bu.Data(); //ppRef
            if (path_to_file.Contains("PbPb")) { 
                ACCcuts = ACCcuts_PbPb_Bu.Data();
                SELcuts = SELcuts_PbPb_Bu.Data();
                trgmatches = "1";
            }
        }
        else {
            ACCcuts    = ACCcuts_ppRef.Data(); //ppRef
            SELcuts    = SELcuts_ppRef.Data(); //ppRef
            if (path_to_file.Contains("PbPb")) { 
                ACCcuts = ACCcuts_PbPb.Data();
                SELcuts = SELcuts_PbPb.Data();
                trgmatches = "1";
            }
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
        if (path_to_file.Contains("Bs")){ 
            sepcCASES = "abs(Btktkmass - 1.019455) < 0.015"; // phi meson mass cut
        } else if (path_to_file.Contains("Bd")){ 
            sepcCASES = "abs(Btktkmass - 0.89594) < 0.25"; // Kstar meson mass cut
        } 

        treeMix->Draw(Form("%s >> hist_SIG", var.Data()), Form(" %s && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));  // SIG
        if (path_to_file.Contains("Bd")){ 
            treeMix->Draw(Form("%s >> hist_SIG_WT"  , var.Data()), Form(" (Bgen == 41000) && %s && %s", cut.Data(), sepcCASES.Data()));                              // WT component
            treedata->Draw(Form("%s >> hist_BKG"     , var.Data()), Form(" (Bmass < 5.162) || (Bmass > 5.400) && %s && %s", cut.Data(), sepcCASES.Data()));       // BKG -- (notice the *!* in the first %s)
            treeMix->Draw(Form("%s >> hist_SIG_BOTH", var.Data()), Form("(%s || (Bgen == 41000)) && %s && %s", isMCsignal.Data(), cut.Data(), sepcCASES.Data()));  // SIG + WT
        } else {
            //treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("((Bmass < 5.289) || (Bmass > 5.449)) && %s && %s", cut.Data(), sepcCASES.Data()) ); 
             // BKG -- (notice the *!* in the first %s) Bs
            treedata->Draw(Form("%s >> hist_BKG", var.Data()), Form("((Bmass < 5.175) || (Bmass > 5.387)) && %s && %s", cut.Data(), sepcCASES.Data()));
            //Bu
        }
        //treeMix->Draw(Form("%s >> hist"    , var.Data()), Form(" %s && %s", cut.Data(), sepcCASES.Data()) );                          // ALL 
        
        //SELECT THE acc + presel CUT 
        ///////////////////////////////////////////////////////////////////////////////////////////////////////// 

        // Customize the Histograms
        hist->SetLineColor(kBlack);
        hist->SetLineWidth(2);

        hist_SIG->SetLineColor(kOrange+7);
        hist_SIG->SetFillColor(kOrange+7);    
        //hist_SIG->SetFillStyle(3001); 
        //hist_SIG->SetLineWidth(2);
        //hist_SIG->SetLineStyle(2);

        hist_SIG_BOTH->SetLineColor(kOrange+7);
        hist_SIG_BOTH->SetFillColor(kOrange+7);
        hist_SIG_WT->SetLineColor(kOrange);
        hist_SIG_WT->SetFillColor(kOrange);    

        hist_BKG->SetLineColor(kBlue);
        hist_BKG->SetFillColor(kBlue);     
        hist_BKG->SetFillStyle(3358);
        //hist_BKG->SetLineStyle(2);
        //hist_BKG->SetLineWidth(2);

        //hist_BKG->SetMarkerStyle(20); // Circle marker
        //hist_BKG->SetMarkerSize(.8); // Bigger dots
        // Customize the Histograms

        if(SELplots==1){ //NORMALIZE
            double nEntries_sig = hist_SIG->GetEntries();
            double nEntries_bkg = hist_BKG->GetEntries();
            double nEntries_sig_WT = hist_SIG_BOTH->GetEntries();

            //double nEntries = hist->GetEntries();
            if (nEntries_sig > 0) {
                hist_SIG->Scale(1.0 / nEntries_sig);
                hist_BKG->Scale(1.0 / nEntries_bkg);
                hist_SIG_BOTH->Scale(1.0 / nEntries_sig_WT);
            }
        }

        if(1){// set the y-axis maximum if needed
            Double_t     max_val = TMath::Max(hist->GetMaximum(), TMath::Max(hist_BKG->GetMaximum(), hist_SIG->GetMaximum()));
            if(SELplots) {
                if (path_to_file.Contains("Bd")) {
                    max_val = TMath::Max( hist_BKG->GetMaximum(), hist_SIG_BOTH->GetMaximum()) ;
                    hist_SIG_BOTH->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                } else {
                    max_val = TMath::Max( hist_SIG->GetMaximum(), hist_BKG->GetMaximum());
                    hist_SIG->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                }
                hist_BKG->SetMaximum(max_val * 1.1); 
            } else {
                hist_SIG->SetMaximum(max_val * 1.1);  // Increase the max range to give some space
                hist_BKG->SetMaximum(max_val * 1.1);
            }
        }

        // Draw the histograms
         hist->SetStats(0);
        if (SELplots && path_to_file.Contains("Bd")){
            hist_SIG_BOTH->Draw("HIST");
        } else {
            hist_SIG->Draw("HIST");
            if (path_to_file.Contains("Bd")) {
                hist_SIG_WT->Draw("HIST SAMES");
            }
        }
        hist_BKG->Draw("HIST SAMES");

        if(!SELplots) hist->Draw("HIST SAME");
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
        }
        TPaveStats *st_sig = (TPaveStats*)hist_SIG->GetListOfFunctions()->FindObject("stats");
        if (st_sig) {
            st_sig->SetTextColor(kOrange+7);
            st_sig->SetLineColor(kOrange+7);
            st_sig->SetX1NDC(0.75);
            st_sig->SetX2NDC(0.95);
            st_sig->SetY1NDC(0.75);
            st_sig->SetY2NDC(0.85);
            st_sig->Draw();
        }
        TPaveStats *st_sigWT = (TPaveStats*)hist_SIG_WT->GetListOfFunctions()->FindObject("stats");
        if (st_sigWT) {
            st_sigWT->SetTextColor(kOrange);
            st_sigWT->SetLineColor(kOrange);
            st_sigWT->SetX1NDC(0.75);
            st_sigWT->SetX2NDC(0.95);
            st_sigWT->SetY1NDC(0.65);
            st_sigWT->SetY2NDC(0.75);
            st_sigWT->Draw();
        }        
         TPaveStats *st_sig_both = (TPaveStats*)hist_SIG_BOTH->GetListOfFunctions()->FindObject("stats");
        if (st_sig_both) {
            st_sig_both->SetTextColor(kOrange+7);
            st_sig_both->SetLineColor(kOrange+7);
            st_sig_both->SetX1NDC(0.75);
            st_sig_both->SetX2NDC(0.95);
            st_sig_both->SetY1NDC(0.75);
            st_sig_both->SetY2NDC(0.85);
            st_sig_both->Draw();
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
        }
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // Add a legend
        auto legend = new TLegend(0.15, 0.7, 0.25, 0.9);
        legend->AddEntry(hist_SIG, "MC SIG", "l");
        legend->AddEntry(hist_BKG, "MC BKG", "l");
        //legend->Draw();

        // Save the canvas as an image
        TString particleNAME = "Bu";
        TString systemNAME = "MC_ppRef_";
        if (path_to_file.Contains("Bs")){
            particleNAME = "Bs";
        } else if (path_to_file.Contains("Bd")){
            particleNAME = "Bd";
        } 
        if (path_to_file.Contains("PbPb"))  { systemNAME = "MC_PbPb_";}

        canvas->SaveAs(Form("./%s%s%s_%s%s.pdf", dirNAME.Data(), systemNAME.Data() , var.Data(), particleNAME.Data(), cutlevel.Data()));

        // Clean up
        delete hist_SIG;
        delete hist_SIG_WT;
        delete hist_SIG_BOTH;
        delete hist_BKG;
        delete hist;
        delete canvas;
        
    }

}
}


int main() {
    plot_dataMC();
    return 0;
}
