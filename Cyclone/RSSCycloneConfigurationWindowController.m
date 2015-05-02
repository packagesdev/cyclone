
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
	
	_numberFormatter=[[NSNumberFormatter alloc] init];
	
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
	
	[_numberOfCyclonesSlider setIntegerValue:tCycloneSettings.numberOfCyclones];
	[_numberOfCyclonesValueLabel setIntegerValue:tCycloneSettings.numberOfCyclones];
	
	[_numberOfParticlesSlider setIntegerValue:tCycloneSettings.numberOfParticlesPerCyclone];
	[_numberOfParticlesValueLabel setIntegerValue:tCycloneSettings.numberOfParticlesPerCyclone];
	
	[_particleSizeSlider setIntegerValue:tCycloneSettings.particleSize];
	[_particleSizeValueLabel setIntegerValue:tCycloneSettings.particleSize];
	
	[_cycloneComplexitySlider setIntegerValue:tCycloneSettings.cycloneComplexity];
	[_cycloneComplexityValueLabel setIntegerValue:tCycloneSettings.cycloneComplexity];
	
	[_speedSlider setIntegerValue:tCycloneSettings.speed];
	[_speedValueLabel setIntegerValue:tCycloneSettings.speed];
	
	[_stretchButton setState:(tCycloneSettings.stretch==YES) ? NSOnState : NSOffState];
	[_showCurvesButton setState:(tCycloneSettings.showCurves==YES) ? NSOnState : NSOffState];
}

#pragma mark -

- (IBAction)setNumberOfCyclones:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).numberOfCyclones=[sender integerValue];
	
	[_numberOfCyclonesValueLabel setIntegerValue:((RSSCycloneSettings *) sceneSettings).numberOfCyclones];
}

- (IBAction)setNumberOfParticles:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).numberOfParticlesPerCyclone=[sender integerValue];
	
	NSString * tFormattedString=[_numberFormatter stringFromNumber:[NSNumber numberWithUnsignedInteger:((RSSCycloneSettings *) sceneSettings).numberOfParticlesPerCyclone]];
	
	[_numberOfParticlesValueLabel setStringValue:tFormattedString];
}

- (IBAction)setParticleSize:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).particleSize=[sender integerValue];
	
	[_particleSizeValueLabel setIntegerValue:((RSSCycloneSettings *) sceneSettings).particleSize];
}

- (IBAction)setCycloneComplexity:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).cycloneComplexity=[sender integerValue];
	
	[_cycloneComplexityValueLabel setIntegerValue:((RSSCycloneSettings *) sceneSettings).cycloneComplexity];
}

- (IBAction)setSpeed:(id)sender
{
	((RSSCycloneSettings *) sceneSettings).speed=[sender integerValue];
	
	[_speedValueLabel setIntegerValue:((RSSCycloneSettings *) sceneSettings).speed];
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
