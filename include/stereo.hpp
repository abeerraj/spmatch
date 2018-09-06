
#pragma once

#include "utils.hpp"
#include "image.hpp"


/**************************************************************************
* > namespace Params                                                      *
* A container for constants used in the PatchMatch algorithm.             *
*                                                                         *
* NOTE: these were the paper weights:                                     *
*   const double ALFA = 0.9;                                              *
*   const double TAU_COL = 10;                                            *
*   const double TAU_GRAD = 2;                                            *
* However I can't use them, because I don't know the numeric range of the *
* RGB and gradients.                                                      *
**************************************************************************/
namespace Params {

	// Def
	enum class OutOfBounds { REPEAT_PIXEL, BLACK_PIXEL, ZERO_COST, ERROR };
	
	// Math constants
	const double ALFA = 0.6;
	const double TAU_COL = 50;
	const double TAU_GRAD = 30;

	// Flags
	const bool NORMALIZE_GRADIENTS = true; // With this false, TAU_GRAD must
	                                       //   also change
	const OutOfBounds OUT_OF_BOUNDS = OutOfBounds::ERROR;

} // TODO: let to change these parameters from main()


/**************************************************************************
* > class StereoImage                                                     *
* An image and its additional informations used for stereo matching.      *
* The methods implements parts of the PatchMatch Stereo algorithm.        *
* Some methods requires this image to be bound to another one.            *
* The member pixelPlanes is the linear disparity function for each pixel. *
* See the comments in .cpp file                                           *
**************************************************************************/
class StereoImage {

	public:

		enum Side { LEFT, RIGHT };

	private:

		Image image;

		size_t width;
		size_t height;

		Grid<PlaneFunction> pixelPlanes;
		Image gradientX;
		Image gradientY;

		Side side;
		StereoImage* other = nullptr;
	
	public:

		// constr
		StereoImage(const string& imgPath, Side side);
		explicit StereoImage(const StereoImage&) = default;
		StereoImage(StereoImage&&) = default;

		// const methods
		double disparityAt(size_t w, size_t h) const;
		void displayGradients(void) const;
		double pixelDissimilarity(size_t w, size_t h,
				const PlaneFunction& disparity) const;
		double adaptiveWeight(size_t pW, size_t pH, size_t qW, size_t qH); // TODO

		// methods
		void bind(StereoImage* o);
		void unbind(void);
};


/************************************************************************
* > class StereoImagePair                                               *
* Represents a couple of StereoImages. The method computeDisparity()    *
* returns an disparity image computed with PatchMatch Stereo algorithm. *
* See .cpp file                                                         *
************************************************************************/
class StereoImagePair {
	
	private:

		StereoImage leftImg;
		StereoImage rightImg;

	public:

		// constr
		StereoImagePair(const string& leftImgPath, const string& rightImgPath):
				leftImg(leftImgPath, StereoImage::LEFT),
				rightImg(rightImgPath, StereoImage::RIGHT)
		{}
		
		// methods
		Image computeDisparity(void);

};