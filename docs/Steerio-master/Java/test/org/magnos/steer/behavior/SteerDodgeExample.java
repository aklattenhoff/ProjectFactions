package org.magnos.steer.behavior;

import java.awt.Color;

import org.magnos.steer.SteerMath;
import org.magnos.steer.SteerSet;
import org.magnos.steer.spatial.SpatialDatabase;
import org.magnos.steer.spatial.array.SpatialArray;
import org.magnos.steer.test.SteerSprite;
import org.magnos.steer.vec.Vec2;

import com.gameprogblog.engine.Game;
import com.gameprogblog.engine.GameLoop;
import com.gameprogblog.engine.GameLoopVariable;
import com.gameprogblog.engine.GameScreen;
import com.gameprogblog.engine.Scene;


public class SteerDodgeExample extends SteerBasicExample
{
	
	public static void main( String[] args )
	{
		Game game = new SteerDodgeExample( DEFAULT_WIDTH, DEFAULT_HEIGHT );
		GameLoop loop = new GameLoopVariable( 0.1f );
		GameScreen screen = new GameScreen( DEFAULT_WIDTH, DEFAULT_HEIGHT, true, loop, game );
		screen.setBackground( Color.black );
		GameScreen.showWindow( screen, "SteerDodgeExample" );
	}
	
	public SpatialDatabase<Vec2> database;
	
	public SteerDodgeExample( int w, int h )
	{
		super( w, h );
	}
	
	@Override
	public void start( Scene scene )
	{
		database = new SpatialArray<Vec2>( 32 );
		
		newSprite( Color.red, 15, 400, 2000, new SteerSet<Vec2>( 
			new SteerDodge<Vec2>( database, 200.0f ),
			new SteerDrive<Vec2>( 0, 0, 2000 )
		));
		
		for (int i = 0; i < 16; i++) 
		{
			SteerSprite lamb = newSprite( Color.green, 15, 300, 1000, 
				new SteerConstant<Vec2>( new Vec2( SteerMath.randomFloat( -300, 300 ), SteerMath.randomFloat( -300, 300 ) ) )	
			);
			
			lamb.position.set( SteerMath.randomFloat( width ), SteerMath.randomFloat( height ) );
			
			database.add( lamb );
		}
	}

}
