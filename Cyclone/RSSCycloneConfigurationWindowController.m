
#import "RSSCycloneConfigurationWindowController.h"

#import "RSSCycloneSettings.h"

@interface RSSCycloneConfigurationWindowController ()
{
	IBOutlet NSSlider *_numberOfCyclonesSlider;
	IBOutlet NSTextField *_numberOfCyclonesValueLabel;
	
	IBOutlet NSSlider *_cycloneComplexitySlider;
	IBOutlet NSTextField *_cycloneComplexityValueLabel;
	
	
	IBOutlet NSSlider *_numberOfParticlesSlider;
	IBOutlet NSTextField *_numberOfParticlesValueLabel;
	
	IBOutlet NSSlider *_particleSizeSlider;
	IBOutlet NSTextField *_particleSizeValueLabel;
	
	
	IBOutlet NSSlider *_speedSlider;
	IBOutlet NSTextField *_speedValueLabel;
	
	IBOutlet NSButton *_stretchButton;
	IBOutlet NSButton *_showCurvesButton;
	
	NSNumberFormatter * _numberFormatter;
}

- (IBAction)setNumberOfCyclones:(id)sender;
- (IBAction)setCycloneComplexity:(id)sender;

- (IBAction)setNumberOfParticles:(id)sender;
- (IBAction)setParticleSize:(id)sender;

- (IBAction)setSpeed:(id)sender;

- (IBAction)setStretch:(id)sender;
- (IBAction)setShowCurves:(id)sender;

@end

@implementation RSSCycloneConfigurationWindowController

- (void)windowDidLoad
{
	[super windowDidLoad];
	
	_numberFormatter=[NSNumberFormatter new];
	
	if (_numberFormatter!=nil)
	{
		_numberFormatter.hasThousandSeparators=YES;
		_numberFormatter.localizesFormat=YES;
	}
}

- (Class)settingsClass
{
	return [RSSCycloneSettings class];
}

- (void)restoreUI
{
	[super restoreUI];
	
	RSSCycloneSettings * tCycloneSettings=(RSSCycloneSettings *) sceneSettings;
	
	_numberOfCyclonesSlider.integerValue=tCycloneSettings.numberOfCyclones;
	_numberOfCyclonesValueLabel.integerValue=tCycloneSettings.numberOfCyclones;
	
	_numberOfParticlesSlider.integerValue=tCycloneSettings.numberOfParticlesPerCyclone;
	_numberOfParticlesValueLabel.integerValue=tCycloneSettings.numberOfParticlesPerCyclone;
	
	_particleSizeSlider.integerValue=tCycloneSettings.particleSize;
	_particleSizeValueLabel.integerValue=tCycloneSettings.particleSize;
	
	_cycloneComplexitySlider.integerValue=tCycloneSettings.cycloneComplexity;
	_cycloneComplexityValueLabel.integerValue=tCycloneSettings.cycloneComplexity;
	
	_speedSlider.integerValue=tCycloneSettings.speed;
	_speedValueLabel.integerValue=tCycloneSettings.speed;
	
	_stretchButton.state=(tCycloneSettings.stretch==YES) ? NSOnState : NSOffState;
	_showCurvesButton.state=(tCycloneSettings.showCurves==YES) ? NSOnState : NSOffState;
}

#pragma mark -

- (IBAction)setNumberOfCyclones:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).numberOfCyclones=[sender integerValue];
	
	_numberOfCyclonesValueLabel.integerValue=((RSSCycloneSettings *) sceneSettings).numberOfCyclones;
}

- (IBAction)setNumberOfParticles:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).numberOfParticlesPerCyclone=[sender integerValue];
	
	NSString * tFormattedString=[_numberFormatter stringFromNumber:@(((RSSCycloneSettings *) sceneSettings).numberOfParticlesPerCyclone)];
	
	_numberOfParticlesValueLabel.stringValue=tFormattedString;
}

- (IBAction)setParticleSize:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).particleSize=[sender integerValue];
	
	_particleSizeValueLabel.integerValue=((RSSCycloneSettings *) sceneSettings).particleSize;
}

- (IBAction)setCycloneComplexity:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).cycloneComplexity=[sender integerValue];
	
	_cycloneComplexityValueLabel.integerValue=((RSSCycloneSettings *) sceneSettings).cycloneComplexity;
}

- (IBAction)setSpeed:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).speed=[sender integerValue];
	
	_speedValueLabel.integerValue=((RSSCycloneSettings *) sceneSettings).speed;
}

- (IBAction)setStretch:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).stretch=([sender state]==NSOnState);
}

- (IBAction)setShowCurves:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).showCurves=([sender state]==NSOnState);
}

@end
