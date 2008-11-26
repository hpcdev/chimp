#ifndef PARTICLEDB_XMLDOC_H
#define PARTICLEDB_XMLDOC_H


//#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
//#include <libxml/xpathInternals.h>
#include <set>
#include <list>
#include <stdexcept>
#include <sstream>

#include <olson-tools/strutil.h>


namespace particledb { namespace xml {


    /** The error exception class for this little XML library wrapper. */
    struct xml_error : std::runtime_error {
        typedef std::runtime_error super;
        xml_error(const std::string & s) : super(s) {}
    };

    struct nonsingle_result_error : xml_error {
        nonsingle_result_error(const std::string & s) : xml_error(s) {}
    };

    struct no_results : nonsingle_result_error {
        no_results(const std::string & s) : nonsingle_result_error(s) {}
    };

    struct too_many_results : nonsingle_result_error {
        too_many_results(const std::string & s) : nonsingle_result_error(s) {}
    };


    inline std::ostream & operator << (std::ostream & out, const xmlChar * s) {
        out << (const char*)s;
        return out;
    }



    class XMLContext;

    /** The templated parser class defaults to using a stringstream to attempt
     * conversion. */
    template <class A>
    struct parser {
        static A parse(const XMLContext & x);
    };



    /** A simple class to represent a single (node-specific) XML context.  */
    struct XMLContext {
        /** A list of XMLContext instances.
         * @see eval(...)
         * */
        typedef std::list<XMLContext> list;

        xmlXPathContextPtr ctx;
        xmlNodePtr node;

        /** The default constructor creates a contextless instance. */
        XMLContext() : ctx(NULL), node(NULL) {}

        /** Constructor which specifies the context within the inherited XML
         * DOM tree. */
        XMLContext(const XMLContext & x, xmlNodePtr node) :
            ctx(x.ctx), node(node) {}

        /** Constructor which simply wraps around the current context in the
         * DOM tree context object. */
        XMLContext(xmlXPathContextPtr ctx) :
            ctx(ctx), node(ctx->node) {}

        /** Constructor which specifies the context within the XML DOM tree.*/
        XMLContext(xmlXPathContextPtr ctx, xmlNodePtr node) :
            ctx(ctx), node(node) {}

        /** Method to reset/set the context of this instance. */
        void set(xmlXPathContextPtr _ctx, xmlNodePtr _node) {
            ctx = _ctx;
            node = _node;
        }

        /** Method to reset/set the context of this instance. */
        void set(xmlXPathContextPtr _ctx) {
            set(_ctx, _ctx->node);
        }

        /** Assertion which throws an xml_error if the context object pointer
         * is NULL. */
        void assertContext(const std::string & err = "invalid XML context") const
        throw (xml_error) {
            /* Apparently, if node == NULL, then the context is at the root of the
             * tree. */
            if (!ctx /* || !node */)
                throw xml_error(err);
        }

        /** XPath evaluation routine which properly pushes this context to the
         * context object and promptly pops this context back off after the
         * XPath evaluation is done. 
         *
         * @return A libxml2 structure which can hold several results.
         * */
        xmlXPathObjectPtr raw_eval(const std::string & q) const {
            assertContext();
            /*push*/
            xmlNodePtr old = ctx->node; ctx->node = node;
            /* eval */
            xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST q.c_str(), ctx);
            /*pop*/
            ctx->node = old;
            /* finished */
            return xpathObj;
        }

        /** A simpler method to obtain the unformatted results of a particular
         * XPath expression.
         *
         * @return A std::list of XMLContext instances that represent each of
         * the different results of the XPath expression <code>q</code>.
         * */
        XMLContext::list eval(const std::string & q) const {
            XMLContext::list result;
            xmlXPathObjectPtr xpathObj = raw_eval(q);

            if (!xpathObj) {
                xmlXPathFreeObject(xpathObj);
                throw xml_error("xpath expression failed:  '" + q + '\'');
            }

            xmlNodeSetPtr nodes = xpathObj->nodesetval;

            if (!nodes)
                return result;

            for(int i = nodes->nodeNr - 1; i >= 0; i--) {
                XMLContext x(*this, nodes->nodeTab[i]);
                result.push_back(x);
            }
            xmlXPathFreeObject(xpathObj);

            return result;
        }

        /** Search for a specific (single-valued) result of the XPath
         * expression in <code>q</code>.
         * */
        XMLContext find(const std::string & q) const {
            XMLContext::list x_list = eval(q);
            if (x_list.size() == 0)
                throw no_results("query '" + q + "' produced no results. cannot create context");
            else if (x_list.size() >1)
                throw too_many_results("query '" + q + "' produced too many results. cannot create context");
            else
                return x_list.front();
        }

        /** Search for and parse a specific (single-valued) result of the
         * XPath expression in <code>q</code>.
         *
         * @return The parsed value of the resulting XML node where the type
         * of the parsing function depends on the template parameter T.
         * */
        template <class T>
        T query(const std::string & q) const {
            XMLContext x = find(q);
            return parser<T>::parse(x);
        }

        /** Search for and parse a specific (single-valued) result of the
         * XPath expression in <code>q</code>.
         *
         * @return The parsed value of the resulting XML node where the type
         * of the parsing function depends on the template parameter T.
         * */
        template <class T>
        T query(const std::string & q, const T & _default) const {
            try {
                XMLContext x = find(q);
                return parser<T>::parse(x);
            } catch (nonsingle_result_error) {
                return _default;
            }
        }

        /** Attempt to parse the text of this current node. */
        template <class T>
        T parse() const {
            return parser<T>::parse(*this);
        }

        /** Return the unparsed/unformatted text of this current node. */
        std::string text() const {
            std::string retval;
            if( node->type == XML_ELEMENT_NODE ||
                node->type == XML_ATTRIBUTE_NODE ||
                node->type == XML_TEXT_NODE) {

                char * txt = (char*)xmlNodeGetContent(node);
                retval = txt;
                free(txt);
            } else {
                std::ostringstream ostr;
                ostr <<  "node \"" << node->name << "\": type " << node->type;
                retval = ostr.str();
            }
        
            return retval;
        }
    };


    template <class A>
    A parser<A>::parse(const XMLContext & x) {
        try {
            return olson_tools::from_string<A>(x.text());
        } catch (olson_tools::string_error& e) {
            throw xml_error(e.what());
        }
    }


    template <>
    struct parser<std::string> {
        static std::string parse(const XMLContext & x) {
            return x.text();
        }
    };


    class XMLDoc {
      public:
        xmlDocPtr doc;
        XMLContext root_context;

        XMLDoc(const std::string & filename = "") :
            doc(NULL), root_context() {
            if (filename.length() > 0)
                open(filename);
        }

        void open(const std::string & filename) throw (xml_error) {
            close();
            doc = xmlParseFile(filename.c_str());
            if (doc == NULL)
                throw xml_error( "Error: unable to parse file \"" + filename + '\"');

            /* Create xpath evaluation context */
            root_context.set(xmlXPathNewContext(doc));
            try {
                root_context.assertContext("unable to create root XPath context");
            } catch (xml_error & e) {
                close();
                throw e;
            }
        }

        /** Close and cleanup. */
        void close() {
            if (doc)
                xmlFreeDoc(doc); 
            doc = NULL;
            if (root_context.ctx)
                xmlXPathFreeContext(root_context.ctx); 
            root_context.ctx = NULL;
        }

        void assertOpen() const throw (xml_error) {
            if (!doc || !root_context.ctx)
                throw xml_error("xml file not opened");
        }

        template <class T>
        T query(const std::string & q) const {
            assertOpen();
            return root_context.query<T>(q);
        }

        template <class T>
        T query(const std::string & q, const T & _default) const {
            assertOpen();
            return root_context.query(q, _default);
        }

        XMLContext::list eval(const std::string & q) const {
            assertOpen();
            return root_context.eval(q);
        }
    };



    template <class A, class B>
    struct data_point : std::pair<A,B> {
        typedef std::pair<A,B> super;
        data_point() : super() {}
        data_point(const A& a, const B& b) : super(a,b) {}
    };

    template <class A, class B>
    bool operator<(const data_point<A,B> & lhs, const data_point<A,B> & rhs) {
        return lhs.first < rhs.first;
    }

    template <class A, class B>
    std::ostream & operator<<(std::ostream & out, const data_point<A,B> & p) {
        return out << "<data x=\'" << p.first << "\' y=\'" << p.second << "\'/>";
    }


    template <class A, class B>
    struct parser< data_point<A,B> > {
        static data_point<A,B> parse(const XMLContext & x) {
            A a = x.query<A>("@x");
            B b = x.query<B>("@y");
            return data_point<A,B>(a,b);
        }
    };



    template <class A, class B>
    struct data_set : std::set< data_point<A,B> > {};

    template<class A, class B>
    std::ostream & operator<<(std::ostream & out, const data_set<A,B> & data) {
        out << "<dataset>\n";
        typename data_set<A,B>::const_iterator i;
        for (i = data.begin(); i != data.end(); i++)
            out << (*i) << '\n';
        return out << "</dataset>";
    }

    template <class A, class B>
    struct parser< data_set<A,B> > {
        static data_set<A,B> parse(const XMLContext & x) {
            A xscale = x.query<A>("@xscale");
            B yscale = x.query<B>("@yscale");

            XMLContext::list x_list = x.eval("val");
            XMLContext::list::iterator i = x_list.begin();

            data_set<A,B> data;
            for(; i != x_list.end(); i++) {
                XMLContext & x1 = (*i);
                data_point<A,B> dp = x1.parse< data_point<A,B> >();
                dp.first  *= xscale;
                dp.second *= yscale;
                data.insert( dp );
            }

            return data;
        }
    };

    }/* namespace xml */
}/* namespace particledb */

#endif // PARTICLEDB_XMLDOC_H
