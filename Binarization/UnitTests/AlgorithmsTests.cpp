#include "CppUnitTest.h"
#include "../Algorithms.hpp"
#include "../ShafaitCalculator.hpp"
#include "../PNM.hpp"

// Used for expanding macro based directives
#define STR(x) #x
#define EXPAND(x) STR(x)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Binarization::UnitTests
{
	TEST_CLASS(AlgorithmsTests)
	{
		void Compare(const Image& imageA, const Image& imageB)
		{
			Assert::AreEqual(imageA.size, imageB.size);

			const int result = std::memcmp(imageA.data, imageB.data, sizeof(Pixel8) * imageA.size);
			Assert::AreEqual(0, result);
		}

		void Compare(const Image& experement, std::string filePath)
		{
			Image control = PNM::Read(filePath);

			Compare(control, experement);
		}

		// Initialized Objects
		static Image image;
		static std::string projFolder;

	public:

		TEST_CLASS_INITIALIZE(Initialize)
		{
			// Obtain the project folder
			// Note: Tests are to be ran in VS.  If this is not the case, change how this is being set.
			//       This was done to avoid baking in the images into the .exe, or copying them on build.
			projFolder = EXPAND(UNITTESTPRJ);
			projFolder.erase(0, 1); // Erase first quote
			projFolder.erase(projFolder.size() - 2); // Erase first quote and period, which is used to excape the trailing slash

			// Load Color Image
			const std::string filePath = projFolder + "2JohnC1V3.pam";
			image = PNM::Read(filePath);
		}

		TEST_METHOD(AlgorithmsSauvolaTest)
		{
			// Run Sauvola algorithms with different calculators
			Image imageSauvolaShafait = Sauvola::ToBinaryImage<Shafait>(image, 26, 0.11);
			Image imageSauvolaShafaitLowMem = Sauvola::ToBinaryImage<Shafait_LowMem>(image, 26, 0.11);

			// Are both equal
			Compare(imageSauvolaShafait, imageSauvolaShafaitLowMem);

			// Do they match our example
			Compare(imageSauvolaShafaitLowMem, projFolder + "2JohnC1V3-Sauvola.pam");
		}

		TEST_METHOD(AlgorithmsNiblackTest)
		{
			Image imageNiblackShafait = Niblack::ToBinaryImage<Shafait>(image, 124, -0.65);

			Compare(imageNiblackShafait, projFolder + "2JohnC1V3-Niblack.pam");
		}

		TEST_METHOD(AlgorithmsWolfTest)
		{
			Image imageWolfShafait = Wolf::ToBinaryImage<Shafait>(image, 20, 0.19);

			Compare(imageWolfShafait, projFolder + "2JohnC1V3-Wolf.pam");
		}

		TEST_METHOD(AlgorithmsNICKTest)
		{
			Image imageNICKShafait = Nick::ToBinaryImage<Shafait>(image, 42, -0.11);

			PNM::Write(imageNICKShafait, projFolder + "2JohnC1V3-NICK-1.pgm");

			Compare(imageNICKShafait, projFolder + "2JohnC1V3-NICK.pam");
		}
	};

	// Static objects
	Image UnitTests::AlgorithmsTests::image;
	std::string UnitTests::AlgorithmsTests::projFolder;
}
