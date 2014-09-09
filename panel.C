// Mainframe macro generated from application: /home/edmsoft/root/root-v5-34-09/bin/root.exe
// By ROOT version 5.34/09 on 2014-09-05 21:03:32

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TRootBrowser
#include "TRootBrowser.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCommandPlugin
#include "TGCommandPlugin.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGTextEdit
#include "TGTextEdit.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGView
#include "TGView.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGFileBrowser
#include "TGFileBrowser.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGTextEditor
#include "TGTextEditor.h"
#endif
#ifndef ROOT_TRootCanvas
#include "TRootCanvas.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#ifndef ROOT_TGHtmlBrowser
#include "TGHtmlBrowser.h"
#endif

#include "Riostream.h"

void panel()
{

   // main frame
   TGMainFrame *fMainFrame1770 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame1770->SetName("fMainFrame1770");

   // horizontal frame
   TGHorizontalFrame *fHorizontalFrame1364 = new TGHorizontalFrame(fMainFrame1770,488,370,kHorizontalFrame);
   fHorizontalFrame1364->SetName("fHorizontalFrame1364");
   TGTextButton *fTextButton1369 = new TGTextButton(fHorizontalFrame1364,"fTextButton1369");
   fTextButton1369->SetTextJustify(36);
   fTextButton1369->SetMargins(0,0,0,0);
   fTextButton1369->SetWrapLength(-1);
   fTextButton1369->Resize(97,22);
   fHorizontalFrame1364->AddFrame(fTextButton1369, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   // "fGroupFrame1501" group frame
   TGGroupFrame *fGroupFrame1501 = new TGGroupFrame(fHorizontalFrame1364,"fGroupFrame1501");
   fGroupFrame1501->SetLayoutBroken(kTRUE);

   fGroupFrame1501->SetLayoutManager(new TGVerticalLayout(fGroupFrame1501));
   fGroupFrame1501->Resize(144,136);
   fHorizontalFrame1364->AddFrame(fGroupFrame1501, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   fMainFrame1770->AddFrame(fHorizontalFrame1364, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));

   fMainFrame1770->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame1770->MapSubwindows();

   fMainFrame1770->Resize(fMainFrame1770->GetDefaultSize());
   fMainFrame1770->MapWindow();
   fMainFrame1770->Resize(490,372);
}  
