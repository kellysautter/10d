package zeidon;

public class zQual extends zView
{
   public zQual( String strSessionId, zView vSubtask )
   {
      super( strSessionId );

      SfActivateSysEmptyOI( m_strSessionId, "KZDBHQUA", vSubtask, zView.zMULTIPLE );
      SetAutoDrop( );
   }

   public void CreateEntitySpec( String EntityName )
   {
      CreateEntity( m_strSessionId, "EntitySpec", zView.zPOS_LAST );
      SetAttribute( m_strSessionId, "EntitySpec", "EntityName", EntityName );
   }

   public void CreateQualAttrib( String EntityName,
                                 String AttributeName,
                                 String Oper,
                                 String Value )
   {
      CreateEntity( m_strSessionId, "QualAttrib", zView.zPOS_LAST );
      SetAttribute( m_strSessionId, "QualAttrib", "EntityName", EntityName );
      SetAttribute( m_strSessionId, "QualAttrib", "AttributeName", AttributeName );
      SetAttribute( m_strSessionId, "QualAttrib", "Oper", Oper );
      SetAttribute( m_strSessionId, "QualAttrib", "Value", Value );
   }
}
