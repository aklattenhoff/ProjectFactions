
package org.magnos.steer.behavior;

import java.awt.Color;
import java.awt.Graphics2D;

import org.magnos.steer.SteerSet;
import org.magnos.steer.target.TargetLocal;
import org.magnos.steer.test.SteerSprite;
import org.magnos.steer.vec.Vec2;

import com.gameprogblog.engine.Game;
import com.gameprogblog.engine.GameLoop;
import com.gameprogblog.engine.GameLoopVariable;
import com.gameprogblog.engine.GameScreen;
import com.gameprogblog.engine.GameState;
import com.gameprogblog.engine.Scene;


public class SteerAwayExample extends SteerBasicExample
{

	public static void main( String[] args )
	{
		Game game = new SteerAwayExample( DEFAULT_WIDTH, DEFAULT_HEIGHT );
		GameLoop loop = new GameLoopVariable( 0.1f );
		GameScreen screen = new GameScreen( DEFAULT_WIDTH, DEFAULT_HEIGHT, true, loop, game );
		screen.setBackground( Color.black );
		GameScreen.showWindow( screen, "SteerAwayExample" );
	}

	private SteerSprite scared;
	private TargetLocal<Vec2> local;

	public SteerAwayExample(int w, int h)
	{
		super( w, h );
	}
	
	@Override
	public void start( Scene scene )
	{
		SteerSprite sprite = newSprite( Color.blue, 15, 300, 1000, new SteerWander2( 0, 100, 150, 80 ) );
		
		scared = newSprite( Color.orange, 15, 300, 1000, new SteerSet<Vec2>(
			new SteerAway<Vec2>( local = new TargetLocal<Vec2>( sprite, 300 ) ),
			new SteerDrive<Vec2>( 0, 0, 100 )
		));
	}
	
	@Override
	public void draw( GameState state, Graphics2D gr, Scene scene )
	{
		super.draw( state, gr, scene );

		if ( drawCircles )
		{
			drawCircle( gr, Color.red, scared.position, local.maximum, false );	
		}
	}
	
}
