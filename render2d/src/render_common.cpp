/****************************************************************************
 * Copyright (C) 2009-2011 GGA Software Services LLC
 *
 * This file is part of Indigo toolkit.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "math/algebra.h"
#include "base_cpp/array.h"
#include "base_cpp/obj_array.h"
#include "molecule/molecule.h"
#include "molecule/query_molecule.h"
#include "reaction/reaction.h"
#include "reaction/query_reaction.h"
#include "render_common.h"

using namespace indigo;

namespace indigo
{
// cos(a) to cos(a/2)
double cos2c (const double cs)
{
   return sqrt((1 + cs)/2);
}

// cos(a) to sin(a/2)
double sin2c (const double cs)
{
   return sqrt((1 - cs)/2);
}

// cos(a) to tg(a/2)
double tg2c (const double cs)
{
   return sqrt((1 - cs) / (1 + cs));
}

// cos(a) to ctg(a/2)
double ctg2c (const double cs)
{
   return sqrt((1 + cs) / (1 - cs));
}
}

RenderItem::RenderItem()
{
   clear();
}
void RenderItem::clear()
{
   ritype = RIT_NULL;
   bbp.set(0, 0);
   bbsz.set(0, 0);
   relpos.set(0, 0);
   color = CWC_BASE;
   highlighted = false;
   noBondOffset = false;
}

void TextItem::clear() {
   RenderItem::clear();
   text.clear();
}

void GraphItem::clear() {
   RenderItem::clear();
}

void RenderItemAttachmentPoint::clear() {
   RenderItem::clear();
   number = -1;
}

void RenderItemBracket::clear() {
   p0.set(0,0);
   p1.set(0,0);
   q0.set(0,0);
   q1.set(0,0);
   d.set(0,0);
   n.set(0,0);
   length = 0;
   width = 0;
}

void RenderItemRSiteAttachmentIndex::clear() {
   RenderItem::clear();
   number = -1;
   radius = 0;
}

AtomDesc::AtomDesc()
{
   clear();
}

void AtomDesc::clear ()
{
   showLabel = showHydro = true;
   tibegin = gibegin = -1;
   ticount = gicount = 0;
   attachmentPointBegin = -1;
   attachmentPointCount = 0;
   rSiteAttachmentIndexBegin = -1;
   rSiteAttachmentIndexCount = 0;
   stereoGroupType =
      stereoGroupNumber = -1;
   isRGroupAttachmentPoint = false;
   pseudoAtomStringVerbose = false;
   fixed = false;
   exactChange = false;
   color = CWC_BASE;
   implicit_h = 0;
   hydroPos = HYDRO_POS_RIGHT;
   aam = -1;
   inversion = STEREO_UNMARKED;
   nearbyAtoms.clear();
   list.clear();
   pseudo.clear();
   memset(implHPosWeights, 0, sizeof(implHPosWeights));
   upperSin = lowerSin = rightSin = leftSin = 0;
}

SGroup::SGroup()
{
   clear();
}

void SGroup::clear ()
{
   tibegin = gibegin = bibegin = -1;
   ticount = gicount = bicount = 0;
}

BondEnd::BondEnd ()
{
   clear();
}

void BondEnd::clear ()
{
   lRing = next = -1;
   centered = false;
   prolong = false;
   lang = (float)(2 * M_PI);
   rang = (float)(2 * M_PI);
   rcos = lcos = -1;
   rsin = lsin = 0;
   rnei = lnei = -1;
   offset = 0;
   width = 0;
}

IMPL_ERROR(BondDescr, "molrender bond description");

BondDescr::BondDescr ()
{
   clear();
}

void BondDescr::clear ()
{
   type = -1;
   queryType = -1;
   inRing = false;
   aromRing = false;
   stereoCare = false;
   thickness = 0.0f;
   stereodir = 0;
   cistrans = false;
   centered = false;
   extP = extN = 0;
   bahs = eahs = 0;
   tiTopology = -1;
   topology = 0;
   reactingCenter = RC_UNMARKED;
}

int BondDescr::getBondEnd (int aid) const
{
   if (aid == beg)
      return be1;
   if (aid == end)
      return be2;
   throw Error("atom given is not adjacent to the bond");
}

Ring::Ring ()
{
   clear();
}

void Ring::clear ()
{
   bondEnds.clear();
   angles.clear();
   dblBondCount = 0;
   aromatic = true;
   center.set(0, 0);
   radius = 0;
}

MoleculeRenderData::MoleculeRenderData ()
{
   clear();
}

void MoleculeRenderData::clear ()
{
   atoms.clear();
   bonds.clear();
   bondends.clear();
   graphitems.clear();
   rings.clear();
   textitems.clear();
   aam.clear();
   reactingCenters.clear();
   inversions.clear();
   exactChanges.clear();
   sgroups.clear();
}

RenderSettings::RenderSettings () :
TL_CP_GET(bondDashAromatic),
TL_CP_GET(bondDashAny),
TL_CP_GET(bondDashSingleOrAromatic),
TL_CP_GET(bondDashDoubleOrAromatic)
{
   init(1.0f);
}

void RenderSettings::init (float sf)
{
   bondLineWidth = sf / 30;
   bondSpace = 2.5f * bondLineWidth;

   fzz[FONT_SIZE_LABEL] = bondLineWidth * 12;
   fzz[FONT_SIZE_ATTR] = bondLineWidth * 8;
   fzz[FONT_SIZE_RGROUP_LOGIC] = bondLineWidth * 12;
   fzz[FONT_SIZE_RGROUP_LOGIC_INDEX] = bondLineWidth * 8;
   fzz[FONT_SIZE_INDICES] = bondLineWidth * 6;
   fzz[FONT_SIZE_ATTACHMENT_POINT_INDEX] = bondLineWidth * 6;
   fzz[FONT_SIZE_RSITE_ATTACHMENT_INDEX] = bondLineWidth * 6;
   fzz[FONT_SIZE_COMMENT] = 0; // not used, value taken from RenderOptions.commentFontFactor
   fzz[FONT_SIZE_TITLE] = 0; // not used, value taken from RenderOptions.titleFontFactor
   fzz[FONT_SIZE_DATA_SGROUP] = bondLineWidth * 8;

   upperIndexShift = -0.4f;
   lowerIndexShift = 0.4f;
   boundExtent = 1.3f * bondLineWidth;
   labelInternalOffset = bondLineWidth;
   stereoGroupLabelOffset = 2 * bondLineWidth;
   radicalRightOffset = bondLineWidth / 2;
   radicalRightVertShift = -0.2f;
   radicalTopOffset = 0.8f * bondLineWidth;
   radicalTopDistDot = bondLineWidth;
   radicalTopDistCap = bondLineWidth / 2;
   dashUnit = bondLineWidth;
   eps = 1e-4f;
   cosineTreshold = 0.98f;
   prolongAdjSinTreshold = 0.2f;
   stereoCareBoxSize = bondSpace * 3 + bondLineWidth * 3;
   minBondLength = bondLineWidth * 5;

   graphItemDotRadius = bondLineWidth;
   graphItemCapSlope = 2;
   graphItemCapBase = 0.7f * bondLineWidth;
   graphItemCapWidth = 1.2f * bondLineWidth;
   graphItemDigitWidth = 4.5f * bondLineWidth;
   graphItemDigitHeight = 6.5f * bondLineWidth;
   graphItemSignLineWidth = 0.8f * bondLineWidth;
   graphItemPlusEdge = (graphItemDigitWidth - graphItemSignLineWidth) / 2;

   const int dashDot[] = {5,2,1,2};
   const int dash[] = {3,2};

   bondDashSingleOrAromatic.clear();
   bondDashDoubleOrAromatic.clear();
   bondDashAny.clear();
   bondDashAromatic.clear();
   for (int i = 0; i < NELEM(dashDot); ++i)
   {
      double val = dashDot[i] * dashUnit;
      bondDashSingleOrAromatic.push(val);
      bondDashDoubleOrAromatic.push(val);
   }
   for (int i = 0; i < NELEM(dash); ++i)
   {
      double val = dash[i] * dashUnit;
      bondDashAny.push(val);
      bondDashAromatic.push(val);
   }

   layoutMarginHorizontal = 0.4f;
   layoutMarginVertical = 0.6f;
   plusSize = 0.5;
   metaLineWidth = 1.0 / 16;
   arrowLength = 3 * plusSize;
   arrowHeadWidth = plusSize / 2;
   arrowHeadSize = plusSize / 2;
   equalityInterval = plusSize / 2;
   rGroupIfThenInterval = bondLineWidth * 4;
   neighboringLabelTolerance = 1.3f;
   minSin = 0.6f;
   neighboringAtomDistanceTresholdA = 0.8f;
   neighboringAtomDistanceTresholdB = 0.5f;
}

CanvasOptions::CanvasOptions ()
{
   clear();
}

void CanvasOptions::clear ()
{
   width = height = -1;
   xOffset = yOffset = 0;
   bondLength = -1;
   gridMarginX = gridMarginY = 0;
   marginX = marginY = 0;
   commentOffset = 0;
   commentPos = COMMENT_POS_BOTTOM;
   commentAlign = 0.5f;
   titleAlign = 0.5f;
   titleOffset = 0;
   gridColumnNumber = 1;
   comment.clear();
   titleProp.clear();
   titleProp.appendString("^NAME", true);
}