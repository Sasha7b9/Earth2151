//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// This code was generated by XmlSchemaClassGenerator version 2.0.560.0 using the following command:
// xscgen -o Collada141 --nc=true --sf .\collada_schema_1_4_1_ms.xsd
namespace Collada141
{
    
    
    /// <summary>
    /// <para>This element allows instancing physics model within another physics model, or in a physics scene.</para>
    /// </summary>
    [System.ComponentModel.DescriptionAttribute("This element allows instancing physics model within another physics model, or in " +
        "a physics scene.")]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("XmlSchemaClassGenerator", "2.0.560.0")]
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlTypeAttribute("instance_physics_model", Namespace="http://www.collada.org/2005/11/COLLADASchema", AnonymousType=true)]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlRootAttribute("instance_physics_model", Namespace="http://www.collada.org/2005/11/COLLADASchema")]
    public partial class Instance_Physics_Model
    {
        
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        private System.Collections.ObjectModel.Collection<InstanceWithExtra> _instance_Force_Field;
        
        /// <summary>
        /// <para>The instance_physics_model element may instance any number of force_field elements.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The instance_physics_model element may instance any number of force_field element" +
            "s.")]
        [System.Xml.Serialization.XmlElementAttribute("instance_force_field")]
        public System.Collections.ObjectModel.Collection<InstanceWithExtra> Instance_Force_Field
        {
            get
            {
                return this._instance_Force_Field;
            }
            private set
            {
                this._instance_Force_Field = value;
            }
        }
        
        /// <summary>
        /// <para xml:lang="en">Gets a value indicating whether the Instance_Force_Field collection is empty.</para>
        /// </summary>
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        public bool Instance_Force_FieldSpecified
        {
            get
            {
                return (this.Instance_Force_Field.Count != 0);
            }
        }
        
        /// <summary>
        /// <para xml:lang="en">Initializes a new instance of the <see cref="Instance_Physics_Model" /> class.</para>
        /// </summary>
        public Instance_Physics_Model()
        {
            this._instance_Force_Field = new System.Collections.ObjectModel.Collection<InstanceWithExtra>();
            this._instance_Rigid_Body = new System.Collections.ObjectModel.Collection<Instance_Rigid_Body>();
            this._instance_Rigid_Constraint = new System.Collections.ObjectModel.Collection<Instance_Rigid_Constraint>();
            this._extra = new System.Collections.ObjectModel.Collection<Extra>();
        }
        
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        private System.Collections.ObjectModel.Collection<Instance_Rigid_Body> _instance_Rigid_Body;
        
        /// <summary>
        /// <para>The instance_physics_model element may instance any number of rigid_body elements.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The instance_physics_model element may instance any number of rigid_body elements" +
            ".")]
        [System.Xml.Serialization.XmlElementAttribute("instance_rigid_body")]
        public System.Collections.ObjectModel.Collection<Instance_Rigid_Body> Instance_Rigid_Body
        {
            get
            {
                return this._instance_Rigid_Body;
            }
            private set
            {
                this._instance_Rigid_Body = value;
            }
        }
        
        /// <summary>
        /// <para xml:lang="en">Gets a value indicating whether the Instance_Rigid_Body collection is empty.</para>
        /// </summary>
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        public bool Instance_Rigid_BodySpecified
        {
            get
            {
                return (this.Instance_Rigid_Body.Count != 0);
            }
        }
        
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        private System.Collections.ObjectModel.Collection<Instance_Rigid_Constraint> _instance_Rigid_Constraint;
        
        /// <summary>
        /// <para>The instance_physics_model element may instance any number of rigid_constraint elements.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The instance_physics_model element may instance any number of rigid_constraint el" +
            "ements.")]
        [System.Xml.Serialization.XmlElementAttribute("instance_rigid_constraint")]
        public System.Collections.ObjectModel.Collection<Instance_Rigid_Constraint> Instance_Rigid_Constraint
        {
            get
            {
                return this._instance_Rigid_Constraint;
            }
            private set
            {
                this._instance_Rigid_Constraint = value;
            }
        }
        
        /// <summary>
        /// <para xml:lang="en">Gets a value indicating whether the Instance_Rigid_Constraint collection is empty.</para>
        /// </summary>
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        public bool Instance_Rigid_ConstraintSpecified
        {
            get
            {
                return (this.Instance_Rigid_Constraint.Count != 0);
            }
        }
        
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        private System.Collections.ObjectModel.Collection<Extra> _extra;
        
        /// <summary>
        /// <para>The extra element may appear any number of times.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The extra element may appear any number of times.")]
        [System.Xml.Serialization.XmlElementAttribute("extra")]
        public System.Collections.ObjectModel.Collection<Extra> Extra
        {
            get
            {
                return this._extra;
            }
            private set
            {
                this._extra = value;
            }
        }
        
        /// <summary>
        /// <para xml:lang="en">Gets a value indicating whether the Extra collection is empty.</para>
        /// </summary>
        [System.Xml.Serialization.XmlIgnoreAttribute()]
        public bool ExtraSpecified
        {
            get
            {
                return (this.Extra.Count != 0);
            }
        }
        
        /// <summary>
        /// <para>The url attribute refers to resource.  This may refer to a local resource using a relative URL 
        ///					fragment identifier that begins with the ???#??? character. The url attribute may refer to an external 
        ///					resource using an absolute or relative URL.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The url attribute refers to resource. This may refer to a local resource using a " +
            "relative URL fragment identifier that begins with the ???#??? character. The url att" +
            "ribute may refer to an external resource using an absolute or relative URL.")]
        [System.ComponentModel.DataAnnotations.RequiredAttribute()]
        [System.Xml.Serialization.XmlAttributeAttribute("url")]
        public string Url { get; set; }
        
        /// <summary>
        /// <para>The sid attribute is a text string value containing the sub-identifier of this element. This 
        ///					value must be unique within the scope of the parent element. Optional attribute.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The sid attribute is a text string value containing the sub-identifier of this el" +
            "ement. This value must be unique within the scope of the parent element. Optiona" +
            "l attribute.")]
        [System.Xml.Serialization.XmlAttributeAttribute("sid")]
        public string Sid { get; set; }
        
        /// <summary>
        /// <para>The name attribute is the text string name of this element. Optional attribute.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute("The name attribute is the text string name of this element. Optional attribute.")]
        [System.Xml.Serialization.XmlAttributeAttribute("name")]
        public string Name { get; set; }
        
        /// <summary>
        /// <para>The parent attribute points to the id of a node in the visual scene. This allows a physics model 
        ///					to be instantiated under a specific transform node, which will dictate the initial position and 
        ///					orientation, and could be animated to influence kinematic rigid bodies.</para>
        /// </summary>
        [System.ComponentModel.DescriptionAttribute(@"The parent attribute points to the id of a node in the visual scene. This allows a physics model to be instantiated under a specific transform node, which will dictate the initial position and orientation, and could be animated to influence kinematic rigid bodies.")]
        [System.Xml.Serialization.XmlAttributeAttribute("parent")]
        public string Parent { get; set; }
    }
}
